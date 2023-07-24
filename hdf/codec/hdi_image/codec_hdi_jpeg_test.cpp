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

#include <buffer_handle.h>
#include <gtest/gtest.h>
#include "codec_image_callback.h"
#include "hdf_log.h"
#include "v1_0/include/idisplay_buffer.h"
#include "v1_0/display_composer_type.h"
#include "v1_0/display_buffer_type.h"
#include "v1_0/icodec_image_jpeg.h"
#define HDF_LOG_TAG codec_jpeg_test

using namespace std;
using namespace testing::ext;
using namespace OHOS::HDI::Display::Buffer::V1_0;
using namespace OHOS::HDI::Display::Composer::V1_0;
using namespace OHOS::HDI::Codec::Image::V1_0;
namespace {
constexpr int32_t WIDTH = 640;
constexpr int32_t HEIGHT = 480;
constexpr uint32_t NORMAL_BUFFER_SIZE = 1000;
constexpr uint32_t CODEC_IMAGE_MAX_BUFFER_SIZE = 50 * 1024 *1024;
static OHOS::sptr<ICodecImageJpeg> hdiJpeg_;
static IDisplayBuffer *hdiBuffer_;
class CodecHdiJpegTest : public testing::Test {
public:

    void InitOutBuffer(CodecImageBuffer &outBuffer)
    {
        AllocInfo alloc = {
            .width = WIDTH,
            .height = HEIGHT,
            .usage = HBM_USE_CPU_READ | HBM_USE_CPU_WRITE | HBM_USE_MEM_DMA,
            .format = PIXEL_FMT_YCBCR_420_SP};

        BufferHandle *bufferHandle = nullptr;
        auto err = hdiBuffer_->AllocMem(alloc, bufferHandle);
        if (err != HDF_SUCCESS) {
            return;
        }
        outBuffer.buffer = new NativeBuffer(bufferHandle);
    }

    static void SetUpTestCase()
    {
        hdiJpeg_ = ICodecImageJpeg::Get();
        hdiBuffer_ = IDisplayBuffer::Get();
    }
    static void TearDownTestCase()
    {
        hdiJpeg_ = nullptr;
        hdiBuffer_ = nullptr;
    }
    void SetUp()
    {
        if (hdiJpeg_ != nullptr) {
            hdiJpeg_->JpegInit();
        }
    }
    void TearDown()
    {
        if (hdiJpeg_ != nullptr) {
            hdiJpeg_->JpegDeInit();
        }
    }
};

HWTEST_F(CodecHdiJpegTest, HdfCodecHdiGetImageCapabilityTest_001, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

HWTEST_F(CodecHdiJpegTest, HdfCodecHdiAllocateInBufferTest_001, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}

HWTEST_F(CodecHdiJpegTest, HdfCodecHdiAllocateInBufferTest_002, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, CODEC_IMAGE_MAX_BUFFER_SIZE + 1);
    ASSERT_NE(ret, HDF_SUCCESS);
}

HWTEST_F(CodecHdiJpegTest, HdfCodecHdiAllocateAndFreeInBufferTest_001, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ret = hdiJpeg_->FreeInBuffer(inBuffer);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

HWTEST_F(CodecHdiJpegTest, HdfCodecHdiFreeInBufferTest_001, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    inBuffer.id = -1;
    auto ret = hdiJpeg_->FreeInBuffer(inBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}

HWTEST_F(CodecHdiJpegTest, HdfCodecHdiDoJpegDecodeTest_001, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    OHOS::sptr<ICodecImageCallback> callback;
    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, callback, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);
}

HWTEST_F(CodecHdiJpegTest, HdfCodecHdiDoJpegDecodeTest_002, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE);
    ASSERT_EQ(ret, HDF_SUCCESS);

    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    OHOS::sptr<ICodecImageCallback> callback;
    ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, callback, decInfo);
    EXPECT_TRUE(ret != HDF_SUCCESS);
    ret = hdiJpeg_->FreeInBuffer(inBuffer);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

HWTEST_F(CodecHdiJpegTest, HdfCodecHdiDoJpegDecodeTest_003, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE);
    ASSERT_EQ(ret, HDF_SUCCESS);

    ASSERT_TRUE(hdiBuffer_ != nullptr);
    struct CodecImageBuffer outBuffer;
    InitOutBuffer(outBuffer);

    struct CodecJpegDecInfo decInfo;
    OHOS::sptr<ICodecImageCallback> callback;
    ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, callback, decInfo);
    EXPECT_TRUE(ret != HDF_SUCCESS);
    ret = hdiJpeg_->FreeInBuffer(inBuffer);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

HWTEST_F(CodecHdiJpegTest, HdfCodecHdiDoJpegDecodeTest_004, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE);
    ASSERT_EQ(ret, HDF_SUCCESS);

    ASSERT_TRUE(hdiBuffer_ != nullptr);
    struct CodecImageBuffer outBuffer;
    InitOutBuffer(outBuffer);

    struct CodecJpegDecInfo decInfo;
    OHOS::sptr<ICodecImageCallback> callback = new CodecImageCallback;
    ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, callback, decInfo);
    EXPECT_TRUE(ret == HDF_SUCCESS);
}
}  // namespace
