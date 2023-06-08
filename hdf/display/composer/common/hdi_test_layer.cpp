/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "hdi_test_layer.h"
#include <unistd.h>
#include "hdi_test_device.h"
#include "v1_0/include/idisplay_buffer.h"
#include "v1_0/display_buffer_type.h"
#include "v1_0/display_composer_type.h"

namespace OHOS {
namespace HDI {
namespace Display {
namespace TEST {
using namespace OHOS::HDI::Display::Buffer::V1_0;

HdiGrallocBuffer::HdiGrallocBuffer(uint32_t w, uint32_t h, PixelFormat fmt)
{
    std::shared_ptr<IDisplayBuffer> gralloc = HdiTestDevice::GetInstance().GetGrallocInterface();
    AllocInfo info = { 0 };
    info.width = w;
    info.height = h;
    info.usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE;
    info.format = fmt;

    int ret = gralloc->AllocMem(info, mBufferHandle);
    if (ret != DISPLAY_SUCCESS) {
        DISPLAY_TEST_LOGE("can not alloc memory");
    }
    void* vaddr = gralloc->Mmap(*mBufferHandle);
    if (vaddr == nullptr) {
        DISPLAY_TEST_LOGE("mmap failed");
    }
}

HdiGrallocBuffer::~HdiGrallocBuffer()
{
    if (mBufferHandle != nullptr) {
        std::shared_ptr<IDisplayBuffer> gralloc = HdiTestDevice::GetInstance().GetGrallocInterface();
        if ((mBufferHandle->virAddr != nullptr) && (gralloc != nullptr)) {
            int ret = gralloc->Unmap(*mBufferHandle);
            if (ret != DISPLAY_SUCCESS) {
                DISPLAY_TEST_LOGE("can not ummap buffer handle");
            }
        }
        gralloc->FreeMem(*mBufferHandle);
        mBufferHandle = nullptr;
    }
    if (mReleaseFence != -1) {
        close(mReleaseFence);
    }
}

void HdiGrallocBuffer::SetReleaseFence(int fd)
{
    DISPLAY_TEST_LOGE("the fd is %{public}d", fd);
    if (mReleaseFence != -1) {
        close(mReleaseFence);
        mReleaseFence = -1;
    }
}

void HdiGrallocBuffer::SetAcquirceFence(int fd)
{
    DISPLAY_TEST_LOGE("the fd is %{public}d", fd);
    mAcquireFence = fd;
}

HdiGrallocBuffer* HdiTestLayer::AcquireBackBuffer()
{
    if (!backBuffers_.empty()) {
        if (currentBuffer_ != nullptr) {
            frontBuffers_.emplace(std::move(currentBuffer_));
        }
        currentBuffer_ = std::move(backBuffers_.front());
        backBuffers_.pop();
    }
    return currentBuffer_.get();
}

HdiGrallocBuffer* HdiTestLayer::GetFrontBuffer() const
{
    HdiGrallocBuffer* buffer = nullptr;
    if (!frontBuffers_.empty()) {
        buffer = frontBuffers_.front().get();
    }
    return buffer;
}

HdiGrallocBuffer* HdiTestLayer::GetBackBuffer() const
{
    HdiGrallocBuffer* buffer = nullptr;
    if (!backBuffers_.empty()) {
        buffer = backBuffers_.front().get();
    }
    return buffer;
}

HdiTestLayer::HdiTestLayer(LayerInfo& info, uint32_t id, uint32_t displayId)
    : id_(id), displayID_(displayId), layerInfo_(info)
{}

int32_t HdiTestLayer::Init()
{
    // init the font queue
    DISPLAY_TEST_LOGE();
    const int MAX_BUFFER_COUNT = 3;
    for (int i = 0; i < MAX_BUFFER_COUNT; i++) {
        std::unique_ptr<HdiGrallocBuffer> buffer =
            std::make_unique<HdiGrallocBuffer>(layerInfo_.width, layerInfo_.height, layerInfo_.pixFormat);
        DISPLAY_TEST_CHK_RETURN((buffer->Get() == nullptr), DISPLAY_FAILURE,
            DISPLAY_TEST_LOGE("buffer handle is null"));
        frontBuffers_.emplace(std::move(buffer));
    }
    displayRect_.w = layerInfo_.width;
    displayRect_.h = layerInfo_.height;
    cropRect_ = displayRect_;
    return DISPLAY_SUCCESS;
}


int32_t HdiTestLayer::SwapFrontToBackQ()
{
    DISPLAY_TEST_CHK_RETURN((frontBuffers_.empty()), DISPLAY_FAILURE, DISPLAY_TEST_LOGE("the font buffer is empty"));
    backBuffers_.emplace(std::move(frontBuffers_.front()));
    frontBuffers_.pop();
    return DISPLAY_SUCCESS;
}

int32_t HdiTestLayer::SwapBackToFrontQ()
{
    DISPLAY_TEST_CHK_RETURN((backBuffers_.empty()), DISPLAY_FAILURE, DISPLAY_TEST_LOGE("the font buffer is empty"));
    frontBuffers_.emplace(std::move(backBuffers_.front()));
    backBuffers_.pop();
    return DISPLAY_SUCCESS;
}

void HdiTestLayer::SetLayerPosition(const IRect& rect)
{
    DISPLAY_TEST_LOGE("x : %{public}d y : %{public}d w : %{public}d h : %{public}d", rect.x, rect.y, rect.w, rect.h);
    displayRect_ = rect;
}

void HdiTestLayer::SetLayerCrop(const IRect& rect)
{
    DISPLAY_TEST_LOGE("x : %{public}d y : %{public}d w : %{public}d h : %{public}d", rect.x, rect.y, rect.w, rect.h);
    cropRect_ = rect;
}

int32_t HdiTestLayer::PreparePresent()
{
    int ret;
    DISPLAY_TEST_LOGE();
    ret = HdiTestDevice::GetInstance().GetDeviceInterface()->SetLayerRegion(displayID_, id_, displayRect_);
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_TEST_LOGE("set display rect failed"));

    ret = HdiTestDevice::GetInstance().GetDeviceInterface()->SetLayerCrop(displayID_, id_, cropRect_);
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_TEST_LOGE("set display crop failed"));

    ret = HdiTestDevice::GetInstance().GetDeviceInterface()->SetLayerZorder(displayID_, id_, zorder_);
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_TEST_LOGE("set display zorder failed"));

    ret = HdiTestDevice::GetInstance().GetDeviceInterface()->SetLayerCompositionType(displayID_, id_, compType_);
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE,
        DISPLAY_TEST_LOGE("set display composition type failed"));

    ret = HdiTestDevice::GetInstance().GetDeviceInterface()->SetLayerTransformMode(displayID_, id_, transform_);
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_TEST_LOGE("set transform mode failed"));

    ret = HdiTestDevice::GetInstance().GetDeviceInterface()->SetLayerAlpha(displayID_, id_, alpha_);
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_TEST_LOGE("set alpha failed"));

    HdiGrallocBuffer* buffer = AcquireBackBuffer();
    DISPLAY_TEST_CHK_RETURN((buffer == nullptr), DISPLAY_FAILURE, DISPLAY_TEST_LOGE("can not get back buffer"));

    BufferHandle* handle = buffer->Get();
    DISPLAY_TEST_CHK_RETURN((handle == nullptr), DISPLAY_FAILURE, DISPLAY_TEST_LOGE("BufferHandle is null"));

    IRect tmp {0, 0, handle->width, handle->height};
    std::vector<IRect> vRects;
    vRects.push_back(tmp);
    ret = HdiTestDevice::GetInstance().GetDeviceInterface()->SetLayerVisibleRegion(displayID_, id_, vRects);
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE,
        DISPLAY_TEST_LOGE("SetLayerVisibleRegion failed"));

    ret = HdiTestDevice::GetInstance().GetDeviceInterface()->SetLayerDirtyRegion(displayID_, id_, vRects);
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE,
        DISPLAY_TEST_LOGE("SetLayerDirtyRegion failed"));

    ret = HdiTestDevice::GetInstance().GetDeviceInterface()->SetLayerBuffer(displayID_, id_, *handle, -1);
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_TEST_LOGE("set buffer handle failed"));

    ret = HdiTestDevice::GetInstance().GetDeviceInterface()->SetLayerBlendType(displayID_, id_, blendType_);
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_TEST_LOGE("set blend type failed"));
    return DISPLAY_SUCCESS;
}

void HdiTestLayer::SetZorder(uint32_t zorder)
{
    DISPLAY_TEST_LOGE("the zorder is %{public}u", zorder);
    zorder_ = zorder;
}

void HdiTestLayer::SetCompType(CompositionType type)
{
    DISPLAY_TEST_LOGE("layer id %{public}u ,the type is : %{public}d", id_, type);
    compType_ = type;
}

void HdiTestLayer::SetTransform(TransformType transform)
{
    transform_ = transform;
}

void HdiTestLayer::SetAlpha(LayerAlpha alpha)
{
    DISPLAY_TEST_LOGE();
    alpha_ = alpha;
}

void HdiTestLayer::SetBlendType(BlendType type)
{
    DISPLAY_TEST_LOGE("type %{public}d", type);
    blendType_ = type;
}

void HdiTestLayer::SetReleaseFence(int fd)
{
    DISPLAY_TEST_LOGE("layer id %{public}u , fd %{public}d", id_, fd);
    if (currentBuffer_ != nullptr) {
        currentBuffer_->SetReleaseFence(fd);
    }
}

HdiTestLayer::~HdiTestLayer() {}
} // OHOS
} // HDI
} // Display
} // TEST
