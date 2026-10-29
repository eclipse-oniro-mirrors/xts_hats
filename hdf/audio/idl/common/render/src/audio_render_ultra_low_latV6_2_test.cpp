/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include <gtest/gtest.h>
#include "osal_mem.h"
#include "v6_0/audio_types.h"
#include "v6_0/iaudio_manager.h"
#include "v6_0/iaudio_render.h"

using namespace std;
using namespace testing::ext;

namespace {
const int BUFFER_LENGTH = 1024 * 16;
const int DEEP_BUFFER_RENDER_PERIOD_SIZE = 4 * 1024;
const int32_t AUDIO_RENDER_CHANNELCOUNT = 2;
const int32_t AUDIO_SAMPLE_RATE_48K = 48000;
const int32_t MAX_AUDIO_ADAPTER_DESC = 5;
const uint64_t DEFAULT_BUFFER_SIZE = 16384;
} // namespace

#ifndef AUDIO_ULTRA_LOW_LAT
#define AUDIO_ULTRA_LOW_LAT 13
#endif

class AudioUtRenderUltraLowLatV6_2Test : public testing::Test {
public:
    struct IAudioManager *manager_ = nullptr;
    struct AudioAdapterDescriptor descs_[MAX_AUDIO_ADAPTER_DESC];
    struct AudioAdapterDescriptor *desc_ = nullptr;
    struct IAudioAdapter *adapter_ = nullptr;
    struct IAudioRender *render_ = nullptr;
    struct AudioDeviceDescriptor devDescRender_ = {};
    struct AudioSampleAttributes attrsRender_ = {};
    uint32_t renderId_ = 0;
    char *devDescriptorName_ = nullptr;
    uint32_t size_ = MAX_AUDIO_ADAPTER_DESC;
    virtual void SetUp();
    virtual void TearDown();
    uint64_t GetRenderBufferSize();
    void InitUltraLowLatAttrs(struct AudioSampleAttributes &attrs);
    void InitRenderDevDesc(struct AudioDeviceDescriptor &devDesc);
    void FreeAdapterElements(struct AudioAdapterDescriptor *dataBlock, bool freeSelf);
    void ReleaseAllAdapterDescs(struct AudioAdapterDescriptor *descs, uint32_t descsLen);
};

uint64_t AudioUtRenderUltraLowLatV6_2Test::GetRenderBufferSize()
{
    int32_t ret = HDF_SUCCESS;
    uint64_t frameSize = 0;
    uint64_t frameCount = 0;
    uint64_t bufferSize = 0;

    if (render_ == nullptr) {
        return DEFAULT_BUFFER_SIZE;
    }
    ret = render_->GetFrameSize(render_, &frameSize);
    if (ret != HDF_SUCCESS) {
        return DEFAULT_BUFFER_SIZE;
    }
    ret = render_->GetFrameCount(render_, &frameCount);
    if (ret != HDF_SUCCESS) {
        return DEFAULT_BUFFER_SIZE;
    }
    bufferSize = frameCount * frameSize;
    if (bufferSize == 0) {
        bufferSize = DEFAULT_BUFFER_SIZE;
    }
    return bufferSize;
}

void AudioUtRenderUltraLowLatV6_2Test::InitUltraLowLatAttrs(struct AudioSampleAttributes &attrs)
{
    attrs.format = AUDIO_FORMAT_TYPE_PCM_16_BIT;
    attrs.channelCount = AUDIO_RENDER_CHANNELCOUNT;
    attrs.sampleRate = AUDIO_SAMPLE_RATE_48K;
    attrs.interleaved = 0;
    attrs.type = (enum AudioCategory)AUDIO_ULTRA_LOW_LAT;
    attrs.period = DEEP_BUFFER_RENDER_PERIOD_SIZE;
    attrs.frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * AUDIO_RENDER_CHANNELCOUNT;
    attrs.isBigEndian = false;
    attrs.isSignedData = true;
    attrs.startThreshold = DEEP_BUFFER_RENDER_PERIOD_SIZE / (attrs.format * attrs.channelCount);
    attrs.stopThreshold = INT_MAX;
    attrs.silenceThreshold = BUFFER_LENGTH;
}

void AudioUtRenderUltraLowLatV6_2Test::InitRenderDevDesc(struct AudioDeviceDescriptor &devDesc)
{
    devDesc.pins = PIN_OUT_SPEAKER;
    devDescriptorName_ = strdup("cardname");
    devDesc.desc = devDescriptorName_;
    ASSERT_NE(desc_, nullptr);
    ASSERT_NE(desc_->ports, nullptr);
    for (uint32_t index = 0; index < desc_->portsLen; index++) {
        if (desc_->ports[index].dir == PORT_OUT) {
            devDesc.portId = desc_->ports[index].portId;
            return;
        }
    }
}

void AudioUtRenderUltraLowLatV6_2Test::FreeAdapterElements(
    struct AudioAdapterDescriptor *dataBlock, bool freeSelf)
{
    if (dataBlock == nullptr) {
        return;
    }
    OsalMemFree(dataBlock->adapterName);
    OsalMemFree(dataBlock->ports);
    if (freeSelf) {
        OsalMemFree(dataBlock);
    }
}

void AudioUtRenderUltraLowLatV6_2Test::ReleaseAllAdapterDescs(
    struct AudioAdapterDescriptor *descs, uint32_t descsLen)
{
    if (descs == nullptr || descsLen == 0) {
        return;
    }
    for (uint32_t i = 0; i < descsLen; i++) {
        FreeAdapterElements(&descs[i], false);
    }
}

void AudioUtRenderUltraLowLatV6_2Test::SetUp()
{
    manager_ = IAudioManagerGet(false);
    ASSERT_NE(manager_, nullptr);

    ASSERT_EQ(HDF_SUCCESS, manager_->GetAllAdapters(manager_, descs_, &size_));
    ASSERT_NE(descs_, nullptr);
    EXPECT_GE(MAX_AUDIO_ADAPTER_DESC, size_);
    desc_ = &descs_[0];
    ASSERT_EQ(HDF_SUCCESS, manager_->LoadAdapter(manager_, desc_, &adapter_));
    ASSERT_NE(adapter_, nullptr);
    InitRenderDevDesc(devDescRender_);
    InitUltraLowLatAttrs(attrsRender_);

    int32_t ret = adapter_->CreateRender(
        adapter_, &devDescRender_, &attrsRender_, &render_, &renderId_);
    if (ret != HDF_SUCCESS) {
        attrsRender_.format = AUDIO_FORMAT_TYPE_PCM_32_BIT;
        ret = adapter_->CreateRender(
            adapter_, &devDescRender_, &attrsRender_, &render_, &renderId_);
    }
    if (ret != HDF_SUCCESS) {
        attrsRender_.format = AUDIO_FORMAT_TYPE_PCM_16_BIT;
        attrsRender_.type = AUDIO_IN_MEDIA;
        ret = adapter_->CreateRender(
            adapter_, &devDescRender_, &attrsRender_, &render_, &renderId_);
    }
    if (ret != HDF_SUCCESS) {
        GTEST_SKIP() << "CreateRender not supported, skip" << std::endl;
    }
    ASSERT_NE(render_, nullptr);
}

void AudioUtRenderUltraLowLatV6_2Test::TearDown()
{
    if (devDescriptorName_ != nullptr) {
        free(devDescriptorName_);
        devDescriptorName_ = nullptr;
    }
    if (adapter_ != nullptr) {
        adapter_->DestroyRender(adapter_, renderId_);
        render_ = nullptr;
    }
    if (manager_ != nullptr) {
        manager_->UnloadAdapter(manager_, desc_->adapterName);
        adapter_ = nullptr;
        ReleaseAllAdapterDescs(descs_, size_);
        IAudioManagerRelease(manager_, false);
    }
}

/* render ultra low latency cases */
HWTEST_F(AudioUtRenderUltraLowLatV6_2Test, SUB_Driver_Audio_RenderUltraLowLat_0100, TestSize.Level1)
{
    ASSERT_NE(render_, nullptr);
    ASSERT_NE(render_->SetSampleAttributes, nullptr);

    struct AudioSampleAttributes attrs = {};
    InitUltraLowLatAttrs(attrs);

    int32_t ret = render_->SetSampleAttributes(render_, &attrs);
    EXPECT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(AudioUtRenderUltraLowLatV6_2Test, SUB_Driver_Audio_RenderUltraLowLat_0200, TestSize.Level1)
{
    ASSERT_NE(render_, nullptr);
    ASSERT_NE(render_->SetSampleAttributes, nullptr);
    ASSERT_NE(render_->GetSampleAttributes, nullptr);

    struct AudioSampleAttributes setAttrs = {};
    InitUltraLowLatAttrs(setAttrs);

    int32_t ret = render_->SetSampleAttributes(render_, &setAttrs);
    if (ret == HDF_ERR_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(HDF_SUCCESS, ret);

    struct AudioSampleAttributes getAttrs = {};
    ret = render_->GetSampleAttributes(render_, &getAttrs);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

HWTEST_F(AudioUtRenderUltraLowLatV6_2Test, SUB_Driver_Audio_RenderUltraLowLat_0300, TestSize.Level1)
{
    ASSERT_NE(render_, nullptr);
    ASSERT_NE(render_->Start, nullptr);
    ASSERT_NE(render_->Stop, nullptr);

    struct AudioSampleAttributes attrs = {};
    InitUltraLowLatAttrs(attrs);

    int32_t ret = render_->SetSampleAttributes(render_, &attrs);
    if (ret == HDF_ERR_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = render_->Start(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = render_->Stop(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

HWTEST_F(AudioUtRenderUltraLowLatV6_2Test, SUB_Driver_Audio_RenderUltraLowLat_0400, TestSize.Level1)
{
    ASSERT_NE(render_, nullptr);
    ASSERT_NE(render_->RenderFrame, nullptr);

    uint32_t frameLen = (uint32_t)GetRenderBufferSize();
    uint64_t requestBytes = frameLen;

    struct AudioSampleAttributes attrs = {};
    InitUltraLowLatAttrs(attrs);

    int32_t ret = render_->SetSampleAttributes(render_, &attrs);
    if (ret == HDF_ERR_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = render_->Start(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    int8_t *frame = (int8_t *)calloc(1, frameLen);
    EXPECT_NE(nullptr, frame);

    ret = render_->RenderFrame(render_, frame, frameLen, &requestBytes);
    EXPECT_EQ(ret, HDF_SUCCESS);

    render_->Stop(render_);

    if (frame != nullptr) {
        free(frame);
        frame = nullptr;
    }
}

HWTEST_F(AudioUtRenderUltraLowLatV6_2Test, SUB_Driver_Audio_RenderUltraLowLat_0500, TestSize.Level1)
{
    ASSERT_NE(render_, nullptr);
    ASSERT_NE(render_->GetLatency, nullptr);

    struct AudioSampleAttributes attrs = {};
    InitUltraLowLatAttrs(attrs);

    int32_t ret = render_->SetSampleAttributes(render_, &attrs);
    if (ret == HDF_ERR_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = render_->Start(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    uint32_t latency = 0;
    ret = render_->GetLatency(render_, &latency);
    EXPECT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);

    render_->Stop(render_);
}
