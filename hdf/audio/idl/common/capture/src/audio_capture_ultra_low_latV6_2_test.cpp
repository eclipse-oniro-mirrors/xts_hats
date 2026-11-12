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
#include <climits>
#include "osal_mem.h"
#include "v6_2/audio_types.h"
#include "v6_2/iaudio_capture.h"
#include "v6_2/iaudio_manager.h"

using namespace std;
using namespace testing::ext;

namespace {
static const uint32_t MAX_AUDIO_ADAPTER_NUM = 5;
const int DEEP_BUFFER_CAPTURE_PERIOD_SIZE = 4 * 1024;
const int TEST_SAMPLE_RATE_48K = 48000;
const int TEST_CHANNEL_COUNT = 2;
} // namespace

class AudioUtCaptureUltraLowLatV6_2Test : public testing::Test {
public:
    struct IAudioManager *manager_ = nullptr;
    struct IAudioAdapter *adapter_ = nullptr;
    struct AudioAdapterDescriptor *adapterDescs_ = nullptr;
    uint32_t captureId_ = 0;
    uint32_t size_ = MAX_AUDIO_ADAPTER_NUM;
    virtual void SetUp();
    virtual void TearDown();
    void InitUltraLowLatAttrs(struct AudioSampleAttributes &attrs);
    void InitCaptureDevDesc(struct AudioDeviceDescriptor &devDesc);
    void FreeAdapterElements(struct AudioAdapterDescriptor *dataBlock, bool freeSelf);
    void ReleaseAllAdapterDescs(struct AudioAdapterDescriptor **descs, uint32_t descsLen);
};

void AudioUtCaptureUltraLowLatV6_2Test::InitUltraLowLatAttrs(struct AudioSampleAttributes &attrs)
{
    attrs.format = AUDIO_FORMAT_TYPE_PCM_16_BIT;
    attrs.channelCount = TEST_CHANNEL_COUNT;
    attrs.sampleRate = TEST_SAMPLE_RATE_48K;
    attrs.interleaved = 0;
    attrs.type = (enum AudioCategory)AUDIO_ULTRA_LOW_LAT;
    attrs.period = DEEP_BUFFER_CAPTURE_PERIOD_SIZE;
    attrs.frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT;
    attrs.isBigEndian = false;
    attrs.isSignedData = true;
    attrs.startThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrs.format * attrs.channelCount);
    attrs.stopThreshold = INT_MAX;
    attrs.silenceThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE;
    attrs.streamId = 0;
}

void AudioUtCaptureUltraLowLatV6_2Test::InitCaptureDevDesc(struct AudioDeviceDescriptor &devDesc)
{
    devDesc.pins = (enum AudioPortPin)PIN_IN_MIC;
    devDesc.desc = const_cast<char *>("primary");
    ASSERT_NE(adapterDescs_, nullptr);
    ASSERT_NE(adapterDescs_->ports, nullptr);
    for (uint32_t index = 0; index < adapterDescs_->portsLen; index++) {
        if (adapterDescs_->ports[index].dir == PORT_IN) {
            devDesc.portId = adapterDescs_->ports[index].portId;
            return;
        }
    }
}

void AudioUtCaptureUltraLowLatV6_2Test::FreeAdapterElements(
    struct AudioAdapterDescriptor *dataBlock, bool freeSelf)
{
    if (dataBlock == nullptr) {
        return;
    }
    if (dataBlock->adapterName != nullptr) {
        OsalMemFree(dataBlock->adapterName);
        dataBlock->adapterName = nullptr;
    }
    if (dataBlock->ports != nullptr) {
        OsalMemFree(dataBlock->ports);
    }
    if (freeSelf) {
        OsalMemFree(dataBlock);
    }
}

void AudioUtCaptureUltraLowLatV6_2Test::ReleaseAllAdapterDescs(
    struct AudioAdapterDescriptor **descs, uint32_t descsLen)
{
    if ((descsLen > 0) && (descs != nullptr) && ((*descs) != nullptr)) {
        for (uint32_t i = 0; i < descsLen; i++) {
            FreeAdapterElements(&(*descs)[i], false);
        }
        OsalMemFree(*descs);
        *descs = nullptr;
    }
}

void AudioUtCaptureUltraLowLatV6_2Test::SetUp()
{
    manager_ = IAudioManagerGet(false);
    ASSERT_NE(manager_, nullptr);

    adapterDescs_ = (struct AudioAdapterDescriptor *)OsalMemCalloc(
        sizeof(struct AudioAdapterDescriptor) * MAX_AUDIO_ADAPTER_NUM);
    ASSERT_NE(adapterDescs_, nullptr);

    EXPECT_EQ(HDF_SUCCESS, manager_->GetAllAdapters(manager_, adapterDescs_, &size_));
    if (size_ > MAX_AUDIO_ADAPTER_NUM) {
        ReleaseAllAdapterDescs(&adapterDescs_, MAX_AUDIO_ADAPTER_NUM);
        ASSERT_LT(size_, MAX_AUDIO_ADAPTER_NUM);
    }

    EXPECT_EQ(HDF_SUCCESS, manager_->LoadAdapter(manager_, &adapterDescs_[0], &adapter_));
    ASSERT_NE(adapter_, nullptr);
}

void AudioUtCaptureUltraLowLatV6_2Test::TearDown()
{
    if (manager_ != nullptr) {
        EXPECT_EQ(HDF_SUCCESS, manager_->UnloadAdapter(manager_, adapterDescs_[0].adapterName));
        ReleaseAllAdapterDescs(&adapterDescs_, MAX_AUDIO_ADAPTER_NUM);
        IAudioManagerRelease(manager_, false);
    }
}

HWTEST_F(AudioUtCaptureUltraLowLatV6_2Test, HdfAudioAdapterCreateCaptureUltraLowLat001, TestSize.Level0)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitCaptureDevDesc(devicedesc);
    InitUltraLowLatAttrs(attrs);

    int32_t ret = adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_);
    if (ret == HDF_ERR_NOT_SUPPORT) {
        GTEST_SKIP() << "Not support UltraLowLat capture" << std::endl;
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = adapter_->DestroyCapture(adapter_, captureId_);
    EXPECT_EQ(HDF_SUCCESS, ret);
}
