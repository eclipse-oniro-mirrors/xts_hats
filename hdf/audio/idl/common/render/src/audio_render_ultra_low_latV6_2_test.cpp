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
#include "v6_2/audio_types.h"
#include "v6_2/iaudio_manager.h"
#include "v6_2/iaudio_render.h"

using namespace std;
using namespace testing::ext;

namespace {
const int DEEP_BUFFER_RENDER_PERIOD_SIZE = 4 * 1024;
const int32_t AUDIO_RENDER_CHANNELCOUNT = 2;
const int32_t AUDIO_SAMPLE_RATE_48K = 48000;
const int32_t MAX_AUDIO_ADAPTER_DESC = 5;
} // namespace

class AudioUtRenderUltraLowLatV6_2Test : public testing::Test {
public:
    struct IAudioManager *manager_ = nullptr;
    struct AudioAdapterDescriptor descs_[MAX_AUDIO_ADAPTER_DESC];
    struct AudioAdapterDescriptor *desc_ = nullptr;
    struct IAudioAdapter *adapter_ = nullptr;
    uint32_t renderId_ = 0;
    uint32_t size_ = MAX_AUDIO_ADAPTER_DESC;
    virtual void SetUp();
    virtual void TearDown();
    void InitUltraLowLatAttrs(struct AudioSampleAttributes &attrs);
    void InitRenderDevDesc(struct AudioDeviceDescriptor &devDesc);
    void FreeAdapterElements(struct AudioAdapterDescriptor *dataBlock, bool freeSelf);
    void ReleaseAllAdapterDescs(struct AudioAdapterDescriptor *descs, uint32_t descsLen);
};

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
    attrs.silenceThreshold = 0;
    attrs.streamId = 0;
}

void AudioUtRenderUltraLowLatV6_2Test::InitRenderDevDesc(struct AudioDeviceDescriptor &devDesc)
{
    devDesc.pins = PIN_OUT_SPEAKER;
    devDesc.desc = const_cast<char *>("primary");
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
}

void AudioUtRenderUltraLowLatV6_2Test::TearDown()
{
    if (manager_ != nullptr) {
        manager_->UnloadAdapter(manager_, desc_->adapterName);
        adapter_ = nullptr;
        ReleaseAllAdapterDescs(descs_, size_);
        IAudioManagerRelease(manager_, false);
    }
}

HWTEST_F(AudioUtRenderUltraLowLatV6_2Test, HdfAudioAdapterCreateRenderUltraLowLat001, TestSize.Level0)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitRenderDevDesc(devicedesc);
    InitUltraLowLatAttrs(attrs);

    int32_t ret = adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_);
    if (ret != HDF_SUCCESS) {
        attrs.format = AUDIO_FORMAT_TYPE_PCM_32_BIT;
        ret = adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_);
    }
    if (ret == HDF_ERR_NOT_SUPPORT) {
        GTEST_SKIP() << "Not support UltraLowLat render" << std::endl;
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = adapter_->DestroyRender(adapter_, renderId_);
    EXPECT_EQ(HDF_SUCCESS, ret);
}
