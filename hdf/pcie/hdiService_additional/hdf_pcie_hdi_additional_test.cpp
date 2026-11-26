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

#include "hdf_base.h"
#include "v1_0/ipcie_interface.h"
#include <cstdio>
#include <string>
#include <vector>
#include <gtest/gtest.h>
#include <securec.h>

using namespace OHOS::HDI::Pcie::V1_0;
using namespace testing::ext;

namespace {
    const std::string SANDBOX_ROOT = "/data/service/el1/public/pcie_driver";
    sptr<IPcieInterface> g_pcieInterface = nullptr;

class HatsHdfPcieServiceAdditionalTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HatsHdfPcieServiceAdditionalTest::SetUpTestCase()
{
    g_pcieInterface = IPcieInterface::Get();
}

void HatsHdfPcieServiceAdditionalTest::TearDownTestCase()
{
}

void HatsHdfPcieServiceAdditionalTest::SetUp()
{
    if (g_pcieInterface == nullptr) {
        printf("PcieInterface is not supported\n");
        GTEST_SKIP() << "PcieInterface is not available" << std::endl;
    }
}

void HatsHdfPcieServiceAdditionalTest::TearDown()
{
}

/* *
 * @tc.number: SUB_Driver_Pcie_GetPcieDevices_0200
 * @tc.name  : testhdiServiceGetPcieDevicesTimes001
 * @tc.desc  : Get pcie devices 10 times.
 */
HWTEST_F(HatsHdfPcieServiceAdditionalTest, testhdiServiceGetPcieDevicesTimes001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_pcieInterface);

    size_t deviceNum = 0;
    bool baselineRecorded = false;
    for (int i = 0; i < 10; i++) {
        std::vector<PcieDeviceInfo> devices;
        int32_t ret = g_pcieInterface->GetPcieDevices(devices);
        if (ret != HDF_SUCCESS && devices.empty()) {
            continue;
        }
        if (ret == HDF_SUCCESS && !baselineRecorded) {
            deviceNum = devices.size();
            baselineRecorded = true;
        } else if (baselineRecorded) {
            EXPECT_EQ(deviceNum, devices.size());
        }
        printf("get pcie device list num[%zu] ret[%d]\n", devices.size(), ret);
    }
}

/* *
 * @tc.number: SUB_Driver_Pcie_GetPcieDevices_0300
 * @tc.name  : testhdiServiceGetPcieDevicesFields001
 * @tc.desc  : Get pcie devices and verify fields of every device.
 */
HWTEST_F(HatsHdfPcieServiceAdditionalTest, testhdiServiceGetPcieDevicesFields001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_pcieInterface);

    std::vector<PcieDeviceInfo> devices;
    int32_t ret = g_pcieInterface->GetPcieDevices(devices);

    for (const auto &device : devices) {
        EXPECT_FALSE(device.boardId.empty());
        EXPECT_FALSE(device.sysfsPath.empty());
        if (device.isModaliasValid) {
            EXPECT_FALSE(device.vendorId.empty());
            EXPECT_FALSE(device.deviceId.empty());
            EXPECT_FALSE(device.classCode.empty());
            EXPECT_FALSE(device.modalias.empty());
        }
        printf("boardId[%s], sysfsPath[%s], vendorId[%s], deviceId[%s], classCode[%s], "
               "modalias[%s], boundDriver[%s]\n",
            device.boardId.c_str(), device.sysfsPath.c_str(), device.vendorId.c_str(),
            device.deviceId.c_str(), device.classCode.c_str(), device.modalias.c_str(),
            device.boundDriver.c_str());
    }
    printf("get pcie devices ret[%d], device num[%zu]\n", ret, devices.size());
}

/* *
 * @tc.number: SUB_Driver_Pcie_LoadPcieDrivers_0200
 * @tc.name  : testhdiServiceLoadPcieDriversEmpty001
 * @tc.desc  : Load pcie drivers with an empty request list.
 */
HWTEST_F(HatsHdfPcieServiceAdditionalTest, testhdiServiceLoadPcieDriversEmpty001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_pcieInterface);

    std::vector<PcieDriverLoadItem> loadRequests;
    std::vector<PcieDriverLoadResult> loadResults;
    int32_t ret = g_pcieInterface->LoadPcieDrivers(loadRequests, loadResults);
    if (ret == HDF_SUCCESS) {
        EXPECT_TRUE(loadResults.empty());
    }
}

/* *
 * @tc.number: SUB_Driver_Pcie_LoadPcieDrivers_0300
 * @tc.name  : testhdiServiceLoadPcieDriversInvalidPath001
 * @tc.desc  : Load pcie drivers with a driver path outside the sandbox.
 */
HWTEST_F(HatsHdfPcieServiceAdditionalTest, testhdiServiceLoadPcieDriversInvalidPath001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_pcieInterface);

    std::vector<PcieDriverLoadItem> loadRequests;
    PcieDriverLoadItem item;
    item.driverIdentity.boardId = "0000:01:00.0";
    item.driverIdentity.driverPath = "/system/lib/modules/pcie_test.ko";
    item.driverIdentity.moduleName = "pcie_test";
    item.options = "";
    loadRequests.push_back(item);

    std::vector<PcieDriverLoadResult> loadResults;
    int32_t ret = g_pcieInterface->LoadPcieDrivers(loadRequests, loadResults);
    EXPECT_NE(HDF_SUCCESS, ret);
    if (!loadResults.empty()) {
        ASSERT_EQ(loadRequests.size(), loadResults.size());
        EXPECT_EQ(item.driverIdentity.boardId, loadResults[0].driverIdentity.boardId);
        printf("statusCode[%d], statusMessage[%s]\n",
            loadResults[0].statusCode, loadResults[0].statusMessage.c_str());
    }
}

/* *
 * @tc.number: SUB_Driver_Pcie_LoadPcieDrivers_0400
 * @tc.name  : testhdiServiceLoadPcieDriversInvalidModule001
 * @tc.desc  : Load pcie drivers with an empty module name and an invalid file name.
 */
HWTEST_F(HatsHdfPcieServiceAdditionalTest, testhdiServiceLoadPcieDriversInvalidModule001,
    Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_pcieInterface);

    std::vector<PcieDriverLoadItem> loadRequests;
    PcieDriverLoadItem item;
    item.driverIdentity.boardId = "0000:01:00.0";
    item.driverIdentity.driverPath = SANDBOX_ROOT + "/";
    item.driverIdentity.moduleName = "";
    item.options = "";
    loadRequests.push_back(item);

    std::vector<PcieDriverLoadResult> loadResults;
    int32_t ret = g_pcieInterface->LoadPcieDrivers(loadRequests, loadResults);
    EXPECT_NE(HDF_SUCCESS, ret);
    if (!loadResults.empty()) {
        ASSERT_EQ(loadRequests.size(), loadResults.size());
        EXPECT_EQ(item.driverIdentity.boardId, loadResults[0].driverIdentity.boardId);
        printf("statusCode[%d], statusMessage[%s]\n",
            loadResults[0].statusCode, loadResults[0].statusMessage.c_str());
    }
}

/* *
 * @tc.number: SUB_Driver_Pcie_LoadPcieDrivers_0500
 * @tc.name  : testhdiServiceLoadPcieDriversOptions001
 * @tc.desc  : Load pcie drivers with a valid sandbox path and empty options.
 */
HWTEST_F(HatsHdfPcieServiceAdditionalTest, testhdiServiceLoadPcieDriversOptions001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_pcieInterface);

    std::vector<PcieDriverLoadItem> loadRequests;
    PcieDriverLoadItem item;
    item.driverIdentity.boardId = "0000:01:00.0";
    item.driverIdentity.driverPath = SANDBOX_ROOT + "/pcie_test.ko";
    item.driverIdentity.moduleName = "pcie_test";
    item.options = "";
    loadRequests.push_back(item);

    std::vector<PcieDriverLoadResult> loadResults;
    int32_t ret = g_pcieInterface->LoadPcieDrivers(loadRequests, loadResults);
    EXPECT_TRUE(ret == HDF_SUCCESS || ret == HDF_FAILURE || ret == HDF_ERR_NOPERM);
    if (!loadResults.empty()) {
        ASSERT_EQ(loadRequests.size(), loadResults.size());
        EXPECT_EQ(item.driverIdentity.boardId, loadResults[0].driverIdentity.boardId);
        printf("statusCode[%d], statusMessage[%s]\n",
            loadResults[0].statusCode, loadResults[0].statusMessage.c_str());
    }
    printf("load pcie drivers options ret[%d]\n", ret);
}

/* *
 * @tc.number: SUB_Driver_Pcie_LoadPcieDrivers_0600
 * @tc.name  : testhdiServiceLoadPcieDriversMulti001
 * @tc.desc  : Load pcie drivers with multiple requests and verify the result mapping.
 */
HWTEST_F(HatsHdfPcieServiceAdditionalTest, testhdiServiceLoadPcieDriversMulti001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_pcieInterface);

    std::vector<PcieDriverLoadItem> loadRequests;
    for (int32_t i = 0; i < 5; i++) {
        PcieDriverLoadItem item;
        item.driverIdentity.boardId = "0000:00:0" + std::to_string(i) + ".0";
        item.driverIdentity.driverPath = SANDBOX_ROOT + "/pcie_test" + std::to_string(i) + ".ko";
        item.driverIdentity.moduleName = "pcie_test" + std::to_string(i);
        item.options = "";
        loadRequests.push_back(item);
    }

    std::vector<PcieDriverLoadResult> loadResults;
    int32_t ret = g_pcieInterface->LoadPcieDrivers(loadRequests, loadResults);
    EXPECT_TRUE(ret == HDF_SUCCESS || ret == HDF_FAILURE || ret == HDF_ERR_NOPERM);
    if (!loadResults.empty()) {
        ASSERT_EQ(loadRequests.size(), loadResults.size());
        for (size_t i = 0; i < loadResults.size(); i++) {
            EXPECT_EQ(loadRequests[i].driverIdentity.boardId, loadResults[i].driverIdentity.boardId);
            EXPECT_EQ(loadRequests[i].driverIdentity.moduleName, loadResults[i].driverIdentity.moduleName);
        }
    }
    printf("load pcie drivers multi ret[%d]\n", ret);
}
}