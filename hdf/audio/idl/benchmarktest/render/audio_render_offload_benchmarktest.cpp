/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
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

#include <benchmark/benchmark.h>
#include <climits>
#include <gtest/gtest.h>
#include "hdf_base.h"
#include "osal_mem.h"

#include "v5_0/audio_types.h"
#include "v5_0/iaudio_manager.h"
#include "v5_0/iaudio_render.h"

using namespace std;
using namespace testing::ext;

namespace {
const int BUFFER_LENTH = 1024 * 16;
const int DEEP_BUFFER_RENDER_PERIOD_SIZE = 4 * 1024;
const int32_t AUDIO_RENDER_CHANNELCOUNT = 2;
const int32_t AUDIO_SAMPLE_RATE_48K = 48000;
const int32_t MAX_AUDIO_ADAPTER_DESC = 5;
const int32_t AUDIO_OFFLOAD_BUFFER_SIZE = 100;
const int32_t AUDIO_BIT_RATE = AUDIO_SAMPLE_RATE_48K * 8;
const int32_t AUDIO_BIT_WIDTH = 32;

class AudioRenderOffloadBenchmarkTest : public benchmark::Fixture {
public:
    struct IAudioManager *manager_ = nullptr;
    struct AudioAdapterDescriptor descs_[MAX_AUDIO_ADAPTER_DESC];
    struct AudioAdapterDescriptor *desc_;
    struct IAudioAdapter *adapter_ = nullptr;
    struct IAudioRender *render_ = nullptr;
    struct AudioDeviceDescriptor devDescRender_ = {};
    struct AudioSampleAttributes attrsRender_ = {};
    uint32_t renderId_ = 0;
    char *devDescriptorName_ = nullptr;
    uint32_t size_ = MAX_AUDIO_ADAPTER_DESC;
    virtual void SetUp(const ::benchmark::State &state);
    virtual void TearDown(const ::benchmark::State &state);
    void InitRenderAttrs(struct AudioSampleAttributes &attrs);
    void InitRenderDevDesc(struct AudioDeviceDescriptor &devDesc);
    void FreeAdapterElements(struct AudioAdapterDescriptor *dataBlock, bool freeSelf);
    void ReleaseAllAdapterDescs(struct AudioAdapterDescriptor *descs, uint32_t descsLen);
};

void AudioRenderOffloadBenchmarkTest::InitRenderAttrs(struct AudioSampleAttributes &attrs)
{
    attrs.channelCount = AUDIO_RENDER_CHANNELCOUNT;
    attrs.sampleRate = AUDIO_SAMPLE_RATE_48K;
    attrs.format = AUDIO_FORMAT_TYPE_PCM_32_BIT;
    attrs.interleaved = 0;
    attrs.type = AUDIO_OFFLOAD;
    attrs.period = DEEP_BUFFER_RENDER_PERIOD_SIZE;
    attrs.frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * AUDIO_RENDER_CHANNELCOUNT;
    attrs.isBigEndian = false;
    attrs.isSignedData = true;
    attrs.startThreshold = DEEP_BUFFER_RENDER_PERIOD_SIZE / (attrs.format * attrs.channelCount);
    attrs.stopThreshold = INT_MAX;
    attrs.silenceThreshold = BUFFER_LENTH;
    attrs.offloadInfo.bitRate = AUDIO_BIT_RATE;
    attrs.offloadInfo.bitWidth = AUDIO_BIT_WIDTH;
    attrs.offloadInfo.sampleRate = AUDIO_SAMPLE_RATE_48K;
    attrs.offloadInfo.channelCount = AUDIO_RENDER_CHANNELCOUNT;
    attrs.offloadInfo.format = AUDIO_FORMAT_TYPE_PCM_32_BIT;
}

void AudioRenderOffloadBenchmarkTest::InitRenderDevDesc(struct AudioDeviceDescriptor &devDesc)
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
    free(devDesc.desc);
}

void AudioRenderOffloadBenchmarkTest::FreeAdapterElements(struct AudioAdapterDescriptor *dataBlock, bool freeSelf)
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

void AudioRenderOffloadBenchmarkTest::ReleaseAllAdapterDescs(struct AudioAdapterDescriptor *descs, uint32_t descsLen)
{
    if (descs == nullptr || descsLen == 0) {
        return;
    }

    for (uint32_t i = 0; i < descsLen; i++) {
        FreeAdapterElements(&descs[i], false);
    }
}

void AudioRenderOffloadBenchmarkTest::SetUp(const ::benchmark::State &state)
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
    InitRenderAttrs(attrsRender_);
    (void)adapter_->CreateRender(adapter_, &devDescRender_, &attrsRender_, &render_, &renderId_);
    ASSERT_NE(render_, nullptr);
}

void AudioRenderOffloadBenchmarkTest::TearDown(const ::benchmark::State &state)
{
    ASSERT_NE(devDescriptorName_, nullptr);
    free(devDescriptorName_);
    devDescriptorName_ = NULL;

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

BENCHMARK_F(AudioRenderOffloadBenchmarkTest, SetBufferSize)(benchmark::State &state)
{
    uint32_t size = AUDIO_OFFLOAD_BUFFER_SIZE;
    int32_t ret;
    for (auto _ : state) {
        ret = render_->SetBufferSize(render_, size);
        ASSERT_TRUE(ret == HDF_SUCCESS);
    }
}

BENCHMARK_REGISTER_F(AudioRenderOffloadBenchmarkTest, SetBufferSize)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

} // end of namespace