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
#include "v6_0/iaudio_capture.h"
#include "v6_0/iaudio_manager.h"

using namespace std;
using namespace testing::ext;

namespace {
static const uint32_t MAX_AUDIO_ADAPTER_NUM = 5;
const int BUFFER_LENGTH = 1024 * 16;
const int DEEP_BUFFER_CAPTURE_PERIOD_SIZE = 4 * 1024;
const int DEFAULT_BUFFER_SIZE = 16384;
const int TEST_SAMPLE_RATE_48K = 48000;
const int TEST_CHANNEL_COUNT = 2;
} // namespace

#ifndef AUDIO_ULTRA_LOW_LAT
#define AUDIO_ULTRA_LOW_LAT 13
#endif

class AudioUtCaptureUltraLowLatV6_2Test : public testing::Test {
public:
    struct IAudioManager *manager_ = nullptr;
    struct IAudioAdapter *adapter_ = nullptr;
    struct IAudioCapture *capture_ = nullptr;
    uint32_t captureId_ = 0;
    char *devDescriptorName_ = nullptr;
    struct AudioAdapterDescriptor *adapterDescs_ = nullptr;
    virtual void SetUp();
    virtual void TearDown();
    uint64_t GetCaptureBufferSize();
    void InitCaptureDevDesc(struct AudioDeviceDescriptor &devDesc);
    void InitUltraLowLatAttrs(struct AudioSampleAttributes &attrs);
    void FreeAdapterElements(struct AudioAdapterDescriptor *dataBlock, bool freeSelf);
    void ReleaseAllAdapterDescs(struct AudioAdapterDescriptor **descs, uint32_t descsLen);
};

uint64_t AudioUtCaptureUltraLowLatV6_2Test::GetCaptureBufferSize()
{
    int32_t ret = HDF_SUCCESS;
    uint64_t frameSize = 0;
    uint64_t frameCount = 0;
    uint64_t bufferSize = 0;

    if (capture_ == nullptr) {
        return DEFAULT_BUFFER_SIZE;
    }
    ret = capture_->GetFrameSize(capture_, &frameSize);
    if (ret != HDF_SUCCESS) {
        return DEFAULT_BUFFER_SIZE;
    }
    ret = capture_->GetFrameCount(capture_, &frameCount);
    if (ret != HDF_SUCCESS) {
        return DEFAULT_BUFFER_SIZE;
    }
    bufferSize = frameCount * frameSize;
    if (bufferSize == 0) {
        bufferSize = DEFAULT_BUFFER_SIZE;
    }
    return bufferSize;
}

void AudioUtCaptureUltraLowLatV6_2Test::InitCaptureDevDesc(struct AudioDeviceDescriptor &devDesc)
{
    devDesc.pins = (enum AudioPortPin)PIN_IN_MIC;
    devDescriptorName_ = strdup("cardname");
    devDesc.desc = devDescriptorName_;
    ASSERT_NE(adapterDescs_, nullptr);
    ASSERT_NE(adapterDescs_->ports, nullptr);
    for (uint32_t index = 0; index < adapterDescs_->portsLen; index++) {
        if (adapterDescs_->ports[index].dir == PORT_IN) {
            devDesc.portId = adapterDescs_->ports[index].portId;
            return;
        }
    }
}

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
    attrs.silenceThreshold = BUFFER_LENGTH;
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
    uint32_t size = MAX_AUDIO_ADAPTER_NUM;
    struct AudioDeviceDescriptor devDesc = {};
    struct AudioSampleAttributes attrs = {};

    manager_ = IAudioManagerGet(false);
    ASSERT_NE(manager_, nullptr);

    adapterDescs_ = (struct AudioAdapterDescriptor *)OsalMemCalloc(
        sizeof(struct AudioAdapterDescriptor) * MAX_AUDIO_ADAPTER_NUM);
    ASSERT_NE(adapterDescs_, nullptr);

    EXPECT_EQ(HDF_SUCCESS, manager_->GetAllAdapters(manager_, adapterDescs_, &size));
    if (size > MAX_AUDIO_ADAPTER_NUM) {
        ReleaseAllAdapterDescs(&adapterDescs_, MAX_AUDIO_ADAPTER_NUM);
        ASSERT_LT(size, MAX_AUDIO_ADAPTER_NUM);
    }

    EXPECT_EQ(HDF_SUCCESS, manager_->LoadAdapter(manager_, &adapterDescs_[0], &adapter_));
    if (adapter_ == nullptr) {
        ReleaseAllAdapterDescs(&adapterDescs_, MAX_AUDIO_ADAPTER_NUM);
        EXPECT_NE(adapter_, nullptr);
    }

    InitCaptureDevDesc(devDesc);
    InitUltraLowLatAttrs(attrs);

    int32_t ret = adapter_->CreateCapture(adapter_, &devDesc, &attrs, &capture_, &captureId_);
    if (ret != HDF_SUCCESS) {
        attrs.type = AUDIO_IN_MEDIA;
        ret = adapter_->CreateCapture(adapter_, &devDesc, &attrs, &capture_, &captureId_);
        if (ret != HDF_SUCCESS) {
            (void)manager_->UnloadAdapter(manager_, adapterDescs_[0].adapterName);
            ReleaseAllAdapterDescs(&adapterDescs_, MAX_AUDIO_ADAPTER_NUM);
        }
    }
    ASSERT_NE(capture_, nullptr);
}

void AudioUtCaptureUltraLowLatV6_2Test::TearDown()
{
    if (devDescriptorName_ != nullptr) {
        free(devDescriptorName_);
        devDescriptorName_ = nullptr;
    }
    if (capture_ != nullptr) {
        EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyCapture(adapter_, captureId_));
    }
    if (manager_ != nullptr) {
        EXPECT_EQ(HDF_SUCCESS, manager_->UnloadAdapter(manager_, adapterDescs_[0].adapterName));
        ReleaseAllAdapterDescs(&adapterDescs_, MAX_AUDIO_ADAPTER_NUM);
        IAudioManagerRelease(manager_, false);
    }
}

/* capture ultra low latency cases */
HWTEST_F(AudioUtCaptureUltraLowLatV6_2Test, SUB_Driver_Audio_CaptureUltraLowLat_0100, TestSize.Level1)
{
    ASSERT_NE(capture_, nullptr);
    ASSERT_NE(capture_->SetSampleAttributes, nullptr);

    struct AudioSampleAttributes attrs = {};
    InitUltraLowLatAttrs(attrs);

    int32_t ret = capture_->SetSampleAttributes(capture_, &attrs);
    EXPECT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(AudioUtCaptureUltraLowLatV6_2Test, SUB_Driver_Audio_CaptureUltraLowLat_0200, TestSize.Level1)
{
    ASSERT_NE(capture_, nullptr);
    ASSERT_NE(capture_->SetSampleAttributes, nullptr);
    ASSERT_NE(capture_->GetSampleAttributes, nullptr);

    struct AudioSampleAttributes setAttrs = {};
    InitUltraLowLatAttrs(setAttrs);

    int32_t ret = capture_->SetSampleAttributes(capture_, &setAttrs);
    if (ret == HDF_ERR_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(HDF_SUCCESS, ret);

    struct AudioSampleAttributes getAttrs = {};
    ret = capture_->GetSampleAttributes(capture_, &getAttrs);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_EQ(AUDIO_ULTRA_LOW_LAT, (int)getAttrs.type);
}

HWTEST_F(AudioUtCaptureUltraLowLatV6_2Test, SUB_Driver_Audio_CaptureUltraLowLat_0300, TestSize.Level1)
{
    ASSERT_NE(capture_, nullptr);
    ASSERT_NE(capture_->Start, nullptr);
    ASSERT_NE(capture_->Stop, nullptr);

    struct AudioSampleAttributes attrs = {};
    InitUltraLowLatAttrs(attrs);

    int32_t ret = capture_->SetSampleAttributes(capture_, &attrs);
    if (ret == HDF_ERR_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = capture_->Stop(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

HWTEST_F(AudioUtCaptureUltraLowLatV6_2Test, SUB_Driver_Audio_CaptureUltraLowLat_0400, TestSize.Level1)
{
    ASSERT_NE(capture_, nullptr);
    ASSERT_NE(capture_->CaptureFrame, nullptr);

    uint32_t frameLen = (uint32_t)GetCaptureBufferSize();
    uint64_t requestBytes = frameLen;

    struct AudioSampleAttributes attrs = {};
    InitUltraLowLatAttrs(attrs);

    int32_t ret = capture_->SetSampleAttributes(capture_, &attrs);
    if (ret == HDF_ERR_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    int8_t *frame = (int8_t *)calloc(1, frameLen);
    EXPECT_NE(nullptr, frame);

    ret = capture_->CaptureFrame(capture_, frame, &frameLen, &requestBytes);
    EXPECT_EQ(ret, HDF_SUCCESS);

    capture_->Stop(capture_);

    if (frame != nullptr) {
        free(frame);
        frame = nullptr;
    }
}

HWTEST_F(AudioUtCaptureUltraLowLatV6_2Test, SUB_Driver_Audio_CaptureUltraLowLat_0500, TestSize.Level1)
{
    ASSERT_NE(capture_, nullptr);
    ASSERT_NE(capture_->GetCapturePosition, nullptr);

    uint32_t frameLen = (uint32_t)GetCaptureBufferSize();
    uint64_t requestBytes = frameLen;

    struct AudioSampleAttributes attrs = {};
    InitUltraLowLatAttrs(attrs);

    int32_t ret = capture_->SetSampleAttributes(capture_, &attrs);
    if (ret == HDF_ERR_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    int8_t *frame = (int8_t *)calloc(1, frameLen);
    EXPECT_NE(nullptr, frame);

    ret = capture_->CaptureFrame(capture_, frame, &frameLen, &requestBytes);
    EXPECT_EQ(ret, HDF_SUCCESS);

    uint64_t frames = 0;
    struct AudioTimeStamp time = {};
    ret = capture_->GetCapturePosition(capture_, &frames, &time);
    EXPECT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);

    capture_->Stop(capture_);

    if (frame != nullptr) {
        free(frame);
        frame = nullptr;
    }
}
