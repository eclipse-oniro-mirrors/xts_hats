/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "hdf_log.h"
#include "usbd_device_test.h"
#include "v1_0/iusb_interface.h"
#include "v1_0/usb_types.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;
using namespace OHOS::HDI::Usb::V1_0;

const int SLEEP_TIME = 3;
sptr<UsbSubscriberTest> UsbdDeviceTestAdditional::subscriber_ = nullptr;

namespace {
sptr<IUsbInterface> g_usbInterface = nullptr;

struct UsbDev UsbdDeviceTestAdditional::dev_ = {0, 0};

int32_t SwitchErrCode(int32_t ret)
{
    return ret == HDF_ERR_NOT_SUPPORT ? HDF_SUCCESS : ret;
}

void UsbdDeviceTestAdditional::SetUpTestCase(void)
{
    g_usbInterface = IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        HDF_LOGE("%{public}s:IUsbInterface::Get() failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->SetPortRole(1, 1, 1);
    sleep(SLEEP_TIME);
    HDF_LOGI("UsbdDeviceTestAdditional::[Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ret = SwitchErrCode(ret);
    ASSERT_EQ(0, ret);
    if (ret != 0) {
        exit(0);
    }

    subscriber_ = new UsbSubscriberTest();
    if (g_usbInterface->BindUsbdSubscriber(subscriber_) != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: bind usbd subscriber_ failed\n", __func__);
        exit(0);
    }
    dev_ = {subscriber_->busNum_, subscriber_->devAddr_};

    std::cout << "please connect device, press enter to continue" << std::endl;
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void UsbdDeviceTestAdditional::TearDownTestCase(void) { g_usbInterface->UnbindUsbdSubscriber(subscriber_); }

void UsbdDeviceTestAdditional::SetUp(void) {}

void UsbdDeviceTestAdditional::TearDown(void) {}

/**
 * @tc.number: SUB_USB_DeviceManager_HDI_CompatDeviceTest_0100
 * @tc.name: testHdiUsbDeviceTestOpenDevice001
 * @tc.desc: Opens a USB device to set up a connection. dev ={1, 255}.
 */
HWTEST_F(UsbdDeviceTestAdditional, testHdiUsbDeviceTestOpenDevice001, Function | MediumTest | Level2)
{
    struct UsbDev dev = {1, 255};
    auto ret = g_usbInterface->OpenDevice(dev);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_USB_DeviceManager_HDI_CompatDeviceTest_0200
 * @tc.name: testHdiUsbDeviceTestOpenDevice002
 * @tc.desc: Opens a USB device to set up a connection. dev ={255, 1}.
 */
HWTEST_F(UsbdDeviceTestAdditional, testHdiUsbDeviceTestOpenDevice002, Function | MediumTest | Level2)
{
    struct UsbDev dev = {255, 1};
    auto ret = g_usbInterface->OpenDevice(dev);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_USB_DeviceManager_HDI_CompatDeviceTest_0300
 * @tc.name: testHdiUsbDeviceTestOpenDevice003
 * @tc.desc: Opens a USB device to set up a connection. dev ={255, 100}.
 */
HWTEST_F(UsbdDeviceTestAdditional, testHdiUsbDeviceTestOpenDevice003, Function | MediumTest | Level2)
{
    struct UsbDev dev = {255, 100};
    auto ret = g_usbInterface->OpenDevice(dev);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_USB_DeviceManager_HDI_CompatDeviceTest_0400
 * @tc.name: testHdiUsbDeviceTestOpenDevice004
 * @tc.desc: Opens a USB device to set up a connection. dev ={100, 255}.
 */
HWTEST_F(UsbdDeviceTestAdditional, testHdiUsbDeviceTestOpenDevice004, Function | MediumTest | Level2)
{
    struct UsbDev dev = {100, 255};
    auto ret = g_usbInterface->OpenDevice(dev);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_USB_DeviceManager_HDI_CompatDeviceTest_0500
 * @tc.name: testHdiUsbDeviceTestCloseDevice001
 * @tc.desc: Closes a USB device to release all system resources related to the device. dev ={1, 255}.
 */
HWTEST_F(UsbdDeviceTestAdditional, testHdiUsbDeviceTestCloseDevice001, Function | MediumTest | Level2)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    ASSERT_EQ(0, ret);
    dev = {1, 255};
    ret = g_usbInterface->CloseDevice(dev);
    ASSERT_NE(ret, 0);
    dev = dev_;
    ret = g_usbInterface->CloseDevice(dev);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.number: SUB_USB_DeviceManager_HDI_CompatDeviceTest_0600
 * @tc.name: testHdiUsbDeviceTestCloseDevice002
 * @tc.desc: Closes a USB device to release all system resources related to the device. dev ={255, 1}.
 */
HWTEST_F(UsbdDeviceTestAdditional, testHdiUsbDeviceTestCloseDevice002, Function | MediumTest | Level2)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    ASSERT_EQ(0, ret);
    dev = {255, 1};
    ret = g_usbInterface->CloseDevice(dev);
    ASSERT_NE(ret, 0);
    dev = dev_;
    ret = g_usbInterface->CloseDevice(dev);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.number: SUB_USB_DeviceManager_HDI_CompatDeviceTest_0700
 * @tc.name: testHdiUsbDeviceTestCloseDevice003
 * @tc.desc: Closes a USB device to release all system resources related to the device. dev ={255, 100}.
 */
HWTEST_F(UsbdDeviceTestAdditional, testHdiUsbDeviceTestCloseDevice003, Function | MediumTest | Level2)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    ASSERT_EQ(0, ret);
    dev = {255, 100};
    ret = g_usbInterface->CloseDevice(dev);
    ASSERT_NE(ret, 0);
    dev = dev_;
    ret = g_usbInterface->CloseDevice(dev);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.number: SUB_USB_DeviceManager_HDI_CompatDeviceTest_0800
 * @tc.name: testHdiUsbDeviceTestCloseDevice004
 * @tc.desc: Closes a USB device to release all system resources related to the device. dev ={100, 255}.
 */
HWTEST_F(UsbdDeviceTestAdditional, testHdiUsbDeviceTestCloseDevice004, Function | MediumTest | Level2)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    ASSERT_EQ(0, ret);
    dev = {100, 255};
    ret = g_usbInterface->CloseDevice(dev);
    ASSERT_NE(ret, 0);
    dev = dev_;
    ret = g_usbInterface->CloseDevice(dev);
    ASSERT_EQ(ret, 0);
}
} // namespace