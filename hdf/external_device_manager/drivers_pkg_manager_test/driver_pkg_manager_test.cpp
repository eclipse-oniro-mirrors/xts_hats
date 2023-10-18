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

#include <gtest/gtest.h>
#include <iostream>
#define private public
#include "driver_pkg_manager.h"
#include "ibus_extension.h"
#include "usb_device_info.h"
#undef private

namespace OHOS {
namespace ExternalDeviceManager {
using namespace std;
using namespace testing::ext;
using namespace OHOS::ExternalDeviceManager;

class DriverPkgManagerTest : public testing::Test {
public:
    void SetUp() override {}
    void TearDown() override {}
};

HWTEST_F(DriverPkgManagerTest, SUB_Driver_Ext_PkgManager_0100, TestSize.Level1)
{
    DriverPkgManager &drvPkgMgrInstance = DriverPkgManager::GetInstance();
    bool ret = drvPkgMgrInstance.Init();
    EXPECT_EQ(0, ret);
    cout << "DrvExt_Init_New_BundleTest" << endl;
}

int32_t Fun(int a, int b, const string & c, const string & d)
{
    cout << a << endl;
    cout << b << endl;
    cout << c << endl;
    cout << d << endl;
    return EDM_OK;
}

HWTEST_F(DriverPkgManagerTest, SUB_Driver_Ext_PkgManager_0200, TestSize.Level1)
{
    DriverPkgManager &drvPkgMgrInstance = DriverPkgManager::GetInstance();
    int32_t regist = drvPkgMgrInstance.RegisterOnBundleUpdate(Fun);
    EXPECT_EQ(0, regist);
    cout << "DrvExt_RegisterOnBundleUpdate_Init_Test" << endl;
}

HWTEST_F(DriverPkgManagerTest, SUB_Driver_Ext_PkgManager_0300, TestSize.Level1)
{
    DriverPkgManager &drvPkgMgrInstance = DriverPkgManager::GetInstance();
    std::shared_ptr<DeviceInfo> devInfo = std::make_shared<DeviceInfo>(
    1);
    devInfo->devInfo_.devBusInfo.busType = BUS_TYPE_INVALID;
    std::shared_ptr<BundleInfoNames> bundle = drvPkgMgrInstance.QueryMatchDriver(devInfo);
    EXPECT_EQ(nullptr, bundle);
    cout << "DrvExt_QueryMatch_Illegal_Bus_Test" << endl;
}

class DriverPkgManagerPtrTest : public testing::Test {
public:
    void SetUp() override {}
    void TearDown() override {}
};

HWTEST_F(DriverPkgManagerPtrTest, SUB_Driver_Ext_PkgManager_0400, TestSize.Level1)
{
    DriverPkgManager &drvPkgMgrInstance = DriverPkgManager::GetInstance();
    std::shared_ptr<DeviceInfo> devInfo = std::make_shared<DeviceInfo>(0);
    std::shared_ptr<BundleInfoNames> bundle = drvPkgMgrInstance.QueryMatchDriver(devInfo);
    EXPECT_EQ(nullptr, bundle);
    cout << "DrvExt_QueryMatch_Before_Init_Test" << endl;
}

HWTEST_F(DriverPkgManagerPtrTest, SUB_Driver_Ext_PkgManager_0500, TestSize.Level1)
{
    DriverPkgManager &drvPkgMgrInstance = DriverPkgManager::GetInstance();
    bool ret = drvPkgMgrInstance.Init();
    if (ret != 0) {
        EXPECT_EQ(0, ret);
        return;
    }
    EXPECT_NE(nullptr, &drvPkgMgrInstance);
    cout << "DrvExt_New_Test" << endl;
}

HWTEST_F(DriverPkgManagerPtrTest, SUB_Driver_Ext_PkgManager_0600, TestSize.Level1)
{
    auto drvPkgMgrInstance = &(DriverPkgManager::GetInstance());
    if (drvPkgMgrInstance != nullptr) {
        delete drvPkgMgrInstance;
        drvPkgMgrInstance = nullptr;
        EXPECT_EQ(nullptr, drvPkgMgrInstance);
    }
    cout << "DrvExt_Delete_Test" << endl;
}
}
}