/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#include <iostream>
#include <vector>

#include "UsbSubscriberTest.h"
#include "hdf_log.h"
#include "usbd_request_test.h"
#include "v1_0/iusb_interface.h"
#include "v1_0/usb_types.h"

const int SLEEP_TIME = 3;
const uint8_t INDEX_0 = 0;
const uint8_t INDEX_1 = 1;
const uint8_t INDEX_INVALID = 255;
const uint8_t CONFIG_ID_0 = 0;
const uint8_t CONFIG_ID_INVALID = 222;
const uint8_t BUS_NUM_INVALID = 255;
const uint8_t DEV_ADDR_INVALID = 255;
const uint8_t STRING_ID_INVALID = 233;
const uint32_t MAX_BUFFER_LENGTH = 255;
const int TAG_NUM_10 = 10;
const uint8_t INTERFACEID_OK = 1;
const uint8_t INTERFACEID_INVALID = 255;
const uint8_t POINTID_INVALID = 158;
// data interface have 2 point : 1->bulk_out 2->bulk_in
const uint8_t POINTID_DIR_IN = USB_ENDPOINT_DIR_IN | 2;
const uint8_t POINTID_DIR_OUT = USB_ENDPOINT_DIR_OUT | 1;
const uint8_t INVALID_NUM = 222;
const uint32_t TIME_WAIT = 10000;

UsbDev UsbdRequestTest::dev_ = {0, 0};

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;
using namespace OHOS::HDI::Usb::V1_0;

namespace {
sptr<IUsbInterface> g_usbInterface = nullptr;

void UsbdRequestTest::SetUpTestCase(void)
{
    g_usbInterface = IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        HDF_LOGE("%{public}s:IUsbInterface::Get() failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->SetPortRole(1, 1, 1);
    sleep(SLEEP_TIME);
    HDF_LOGI("UsbdRequestTest::[Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    if (ret != 0) {
        exit(0);
    }

    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    if (g_usbInterface->BindUsbdSubscriber(subscriber) != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: bind usbd subscriber failed", __func__);
        exit(0);
    }

    std::cout << "please connect device, press enter to continue" << std::endl;
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    dev_ = { subscriber->busNum_, subscriber->devAddr_ };

    ret = g_usbInterface->OpenDevice(dev_);
    HDF_LOGI("UsbdRequestTest:: %{public}d OpenDevice=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdRequestTest::TearDownTestCase(void)
{
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    if (g_usbInterface->BindUsbdSubscriber(subscriber) != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: bind usbd subscriber failed", __func__);
        exit(0);
    }
    dev_ = { subscriber->busNum_, subscriber->devAddr_ };
    auto ret = g_usbInterface->CloseDevice(dev_);
    HDF_LOGI("UsbdRequestTest:: %{public}d Close=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdRequestTest::SetUp(void) {}

void UsbdRequestTest::TearDown(void) {}

/**
 * @tc.name: SUB_USB_HDI_0090
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0090, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0090 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0100
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0100, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0100 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0110
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0110, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0110 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0120
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, configIndex error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0120, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_INVALID;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
    configIndex = INDEX_1;
    ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0120 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0130
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0130, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0130 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0140
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, busNum && configIndex error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0140, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_INVALID;
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0140 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0150
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, devAddr && configIndex error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0150, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_INVALID;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0150 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0160
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && configIndex error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0160, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_INVALID;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0160 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HDI_0170
 * @tc.desc: Test functions to GetConfig
 * @tc.desc: int32_t GetConfig(const UsbDev &dev, uint8_t &configIndex);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0170, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0170 %{public}d GetConfig=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0180
 * @tc.desc: Test functions to GetConfig
 * @tc.desc: int32_t GetConfig(const UsbDev &dev, uint8_t &configIndex);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0180, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0180 %{public}d GetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0190
 * @tc.desc: Test functions to GetConfig
 * @tc.desc: int32_t GetConfig(const UsbDev &dev, uint8_t &configIndex);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0190, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0190 %{public}d GetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0200
 * @tc.desc: Test functions to GetConfig
 * @tc.desc: int32_t GetConfig(const UsbDev &dev, uint8_t &configIndex);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0200, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0200 %{public}d GetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1010
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1010, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1010 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1020
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1020, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1020 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1030
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1030, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1030 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1040
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1040, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    interfaceId = INTERFACEID_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1040 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1050
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1050, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1050 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1060
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1060, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1060 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1070
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, devAddr && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1070, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1070 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1080
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1080, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1080 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HDI_1170
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1170, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t altIndex = INDEX_0;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1170 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1170 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1180
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1180, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t altIndex = INDEX_0;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1180 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1180 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1190
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1190, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t altIndex = INDEX_INVALID;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1190 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1190 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1200
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1200, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t altIndex = INDEX_INVALID;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1200 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    interfaceId = INTERFACEID_INVALID;
    ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1200 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1210
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1210, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t altIndex = INDEX_0;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1210 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    dev.devAddr = DEV_ADDR_INVALID;
    ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1210 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1220
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, busNum && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1220, Function | MediumTest | Level1)
{
    int32_t interfaceId = INTERFACEID_OK;
    uint8_t altIndex = INDEX_1;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1220 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    interfaceId = INTERFACEID_INVALID;
    ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1220 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1230
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, devAddr && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1230, Function | MediumTest | Level1)
{
    int32_t interfaceId = INTERFACEID_OK;
    uint8_t altIndex = INDEX_INVALID;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1230 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    interfaceId = INTERFACEID_INVALID;
    ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1230 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1240
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1240, Function | MediumTest | Level1)
{
    uint8_t altIndex = INDEX_INVALID;
    int32_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1240 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    dev.devAddr = DEV_ADDR_INVALID;
    interfaceId = INTERFACEID_INVALID;
    ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1240 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0210
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0210, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0210 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0220
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0220, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0220 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0230
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0230, Function | MediumTest | Level1)
{
    uint8_t devAddr = DEV_ADDR_INVALID;
    struct UsbDev dev = { dev_.busNum, devAddr };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0230 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0240
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, length error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0240, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0240 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0250
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0250, Function | MediumTest | Level1)
{
    uint8_t busNum = BUS_NUM_INVALID;
    uint8_t devAddr = DEV_ADDR_INVALID;
    struct UsbDev dev = { busNum, devAddr };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0250 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0260
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && length error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0260, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0260 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0270
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr && length error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0270, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0270 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0280
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Reverse test: busNum、devAddr、length error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0280, Function | MediumTest | Level1)
{
    uint8_t busNum = BUS_NUM_INVALID;
    uint8_t devAddr = DEV_ADDR_INVALID;
    struct UsbDev dev = { busNum, devAddr };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0280 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HDI_0290
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0290, Function | MediumTest | Level1)
{
    uint8_t stringId = 0;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0290 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0300
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0300, Function | MediumTest | Level1)
{
    uint8_t stringId = 1;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0300 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0310
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, stringId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0310, Function | MediumTest | Level1)
{
    uint8_t stringId = INVALID_NUM;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0310 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0320
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0320, Function | MediumTest | Level1)
{
    uint8_t stringId = 0;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0320 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0330
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0330, Function | MediumTest | Level1)
{
    uint8_t stringId = 0;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0330 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0340
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0340, Function | MediumTest | Level1)
{
    uint8_t stringId = 0;
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0340 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0350
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr && stringID error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0350, Function | MediumTest | Level1)
{
    uint8_t stringId = STRING_ID_INVALID;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0350 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0360
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && stringID error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0360, Function | MediumTest | Level1)
{
    uint8_t stringId = STRING_ID_INVALID;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0360 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HDI_0370
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0370, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_0;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0370 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0380
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0380, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_0;
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0380 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0390
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0390, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_0;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0390 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0400
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, configId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0400, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_0;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0400 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0410
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0410, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_0;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0410 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0420
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum、configId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0420, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_INVALID;
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0420 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0430
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr && configId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0430, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_INVALID;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0430 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0440
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && configId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0440, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_INVALID;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0440 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1710
 * @tc.desc: Test functions to GetRawDescriptor
 * @tc.desc: int32_t GetRawDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1710, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> rawData;
    auto ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1710 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        rawData.size(), sizeof(rawData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1720
 * @tc.desc: Test functions to GetRawDescriptor
 * @tc.desc: int32_t GetRawDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1720, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> rawData;
    auto ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1720 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        rawData.size(), sizeof(rawData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1730
 * @tc.desc: Test functions to GetRawDescriptor
 * @tc.desc: int32_t GetRawDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1730, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> rawData;
    auto ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1730 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        rawData.size(), sizeof(rawData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1740
 * @tc.desc: Test functions to GetFileDescriptor
 * @tc.desc: int32_t GetFileDescriptor(const UsbDev &dev, int32_t &fd);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1740, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    int32_t fd = 0;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1740 %{public}d fd=%{public}d ret=%{public}d", __LINE__, fd, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1750
 * @tc.desc: Test functions to GetFileDescriptor
 * @tc.desc: int32_t GetFileDescriptor(const UsbDev &dev, int32_t &fd);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1750, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    int32_t fd = 0;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1750 %{public}d fd=%{public}d ret=%{public}d", __LINE__, fd, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1760
 * @tc.desc: Test functions to GetFileDescriptor
 * @tc.desc: int32_t GetFileDescriptor(const UsbDev &dev, int32_t &fd);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1760, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    int32_t fd = 0;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1760 %{public}d fd=%{public}d ret=%{public}d", __LINE__, fd, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1770
 * @tc.desc: Test functions to GetFileDescriptor
 * @tc.desc: int32_t GetFileDescriptor(const UsbDev &dev, int32_t &fd);
 * @tc.desc: Negative test: parameters exception, fd error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1770, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    int32_t fd = MAX_BUFFER_LENGTH;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1770 %{public}d fd=%{public}d ret=%{public}d", __LINE__, fd, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0790
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0790, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0790 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0790 interfaceId=%{public}d pointId=%{public}d ret=%{public}d",
        interfaceId, pointId, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0800
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0800, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0800 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0800 interfaceId=%{public}d pointId=%{public}d ret=%{public}d",
        interfaceId, pointId, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0810
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0810, Function | MediumTest | Level1)
{
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0810 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0810 interfaceId=%{public}d pointId=%{public}d ret=%{public}d",
        interfaceId, pointId, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0820
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, busNum && configIndex error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0820, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0820 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    interfaceId = INVALID_NUM;
    dev.busNum = BUS_NUM_INVALID;
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0820 interfaceId=%{public}d pointId=%{public}d ret=%{public}d",
        interfaceId, pointId, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0830
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0830, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0830 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0830 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0840
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, busNum && interfaceId && pointId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0840, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0840 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    interfaceId = INVALID_NUM;
    pointId = INVALID_NUM;
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0840 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0850
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0850, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0850 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'w', 'r', 'i', 't', 'e'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '7'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0850 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0860
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0860, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0860 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    interfaceId = INVALID_NUM;
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'w', 'r', 'i', 't', 'e'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '8'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0860 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0870
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, interfaceId && pointId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0870, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0870 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    interfaceId = INVALID_NUM;
    pointId = INVALID_NUM;
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'w', 'r', 'i', 't', 'e'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '9'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0870 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HDI_0880
 * @tc.desc: Test functions to RequestWait
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0880, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0880 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0880 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> waitData(TAG_NUM_10);
    ret = g_usbInterface->RequestWait(dev, waitData, bufferData, TIME_WAIT);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0880 %{public}d RequestWait=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0890
 * @tc.desc: Test functions to RequestWait
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0890, Function | MediumTest | Level1)
{
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0890 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbPipe pipe = { interfaceId, pointId };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0890 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    std::vector<uint8_t> waitData(TAG_NUM_10);
    ret = g_usbInterface->RequestWait(dev, waitData, bufferData, TIME_WAIT);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0890 %{public}d RequestWait=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0900
 * @tc.desc: Test functions to RequestWait
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0900, Function | MediumTest | Level1)
{
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0900 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbPipe pipe = { interfaceId, pointId };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0900 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    std::vector<uint8_t> waitData(TAG_NUM_10);
    ret = g_usbInterface->RequestWait(dev, waitData, bufferData, TIME_WAIT);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0900 %{public}d RequestWait=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0910
 * @tc.desc: Test functions to RequestWait
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Negative test: parameters exception, timeout error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0910, Function | MediumTest | Level1)
{
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0910 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0910 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> waitData(TAG_NUM_10);
    ret = g_usbInterface->RequestWait(dev, waitData, bufferData, -TIME_WAIT);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0910 %{public}d RequestWait=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0920
 * @tc.desc: Test functions to RequestWait
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0920, Function | MediumTest | Level1)
{
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0920 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0920 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> waitData(TAG_NUM_10);
    dev.devAddr = DEV_ADDR_INVALID;
    dev.busNum = BUS_NUM_INVALID;
    ret = g_usbInterface->RequestWait(dev, waitData, bufferData, TIME_WAIT);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0920 %{public}d RequestWait=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HDI_0930
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0930, Function | MediumTest | Level1)
{
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0930 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '1'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0930 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0930 %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0940
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0940, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0940 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '2'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0940 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0940 %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0940 again %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0950
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0950, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0950 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '3'};
    struct UsbPipe pipe = { interfaceId, pointId };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0950 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0950 %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::UsbdRequestCancel003 again %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0960
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Positive test: intfId && endpointId error in pipe but not used
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0960, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0960 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '4'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0960 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    pipe.intfId = INTERFACEID_INVALID;
    pipe.endpointId = POINTID_INVALID;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0960 %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    pipe = {interfaceId, pointId};
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::UsbdRequestCancel004 %{public}d again RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0970
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: call twice
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0970, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0970 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '5'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0970 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0970 %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev = dev_;
    pipe = {interfaceId, pointId};
    ret = g_usbInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0980
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, busNum error && interfaceid ignore
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0980, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0980 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '6'};
    struct UsbPipe pipe = { interfaceId, pointId };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0980 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    pipe.intfId = 224;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0980 %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev.busNum = dev_.busNum;
    pipe.intfId = INTERFACEID_OK;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0990
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, devAddr error && interfaceid ignore
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0990, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0990 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '7'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0990 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    pipe.intfId = INTERFACEID_INVALID;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0990 %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev.devAddr = dev_.devAddr;
    pipe.intfId = INTERFACEID_OK;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1000
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, busNum error && devAddr error && interfaceid ignore
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1000, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1000 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '8'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1000 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    dev.devAddr = DEV_ADDR_INVALID;
    pipe.intfId = INTERFACEID_INVALID;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1000 %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    pipe.intfId = INTERFACEID_OK;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HDI_1090
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1090, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1090 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1100
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1100, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1100 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1110
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1110, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1110 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1120
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1120, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1120 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1130
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1130, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1130 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1140
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && interfaceid error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1140, Function | MediumTest | Level1)
{
    int32_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = { DEV_ADDR_INVALID, dev_.devAddr };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1140 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1150
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, devAddr && interfaceid error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1150, Function | MediumTest | Level1)
{
    int32_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1150 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1160
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && interfaceid error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1160, Function | MediumTest | Level1)
{
    int32_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1160 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1990
 * @tc.desc: Test functions to BulkCancel
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1990, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    struct UsbPipe pipe = {interfaceId, pointId};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1990 %{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
    ret = g_usbInterface->BulkCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1990 %{public}d BulkCancel=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1990 %{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2000
 * @tc.desc: Test functions to BulkCancel
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_2000, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    struct UsbPipe pipe = {interfaceId, pointId};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2000 %{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
    dev.busNum = BUS_NUM_INVALID;
    ret = g_usbInterface->BulkCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_2000 %{public}d BulkCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(0, ret);
    dev = dev_;
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2000 %{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2010
 * @tc.desc: Test functions to BulkCancel
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_2010, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    struct UsbPipe pipe = {interfaceId, pointId};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2010 %{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
    dev.devAddr = DEV_ADDR_INVALID;
    ret = g_usbInterface->BulkCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_2010 %{public}d BulkCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(0, ret);
    dev = dev_;
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2010 %{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2020
 * @tc.desc: Test functions to BulkCancel
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && interfaceid error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_2020, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    struct UsbPipe pipe = {interfaceId, pointId};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2020 %{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
    dev.busNum = BUS_NUM_INVALID;
    dev.devAddr = DEV_ADDR_INVALID;
    pipe.intfId = POINTID_INVALID;
    ret = g_usbInterface->BulkCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_2020 %{public}d BulkCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(0, ret);
    dev = dev_;
    pipe = {interfaceId, pointId};
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2020 %{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2030
 * @tc.desc: Test functions to BulkCancel
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, intfId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_2030, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    struct UsbPipe pipe = {interfaceId, pointId};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2030 %{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
    pipe.intfId = POINTID_INVALID;
    ret = g_usbInterface->BulkCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_2030 %{public}d BulkCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(0, ret);
    pipe = {interfaceId, pointId};
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2030 %{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}
} // namespace
