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

#include <cstdio>
#include <string>
#include <vector>
#include <gtest/gtest.h>
#include <securec.h>
#include "hdf_base.h"
#include "v1_0/ipcie_interface.h"

using namespace OHOS::HDI::Pcie::V1_0;
using namespace testing::ext;

namespace {
    const std::string SANDBOX_ROOT = "/data/service/el1/public/pcie_driver";
    sptr<IPcieInterface> g_pcieInterface = nullptr;

class HatsHdfPcieServiceTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HatsHdfPcieServiceTest::SetUpTestCase()
{
    g_pcieInterface = IPcieInterface::Get();
}

void HatsHdfPcieServiceTest::TearDownTestCase()
{
}

void HatsHdfPcieServiceTest::SetUp()
{
    if (g_pcieInterface == nullptr) {
        printf("PcieInterface is not supported\n");
        GTEST_SKIP() << "PcieInterface is not available" << std::endl;
    }
}

void HatsHdfPcieServiceTest::TearDown()
{
}

/**
 * @tc.name: CheckPcieInstanceIsEmpty
 * @tc.desc: Create a pcie instance. The instance is not empty.
 * @tc.type: FUNC
 */
HWTEST_F(HatsHdfPcieServiceTest, SUB_Driver_HdiPcie_0100, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_pcieInterface);
}

/**
 * @tc.name: GetPcieDevices001
 * @tc.desc: Get pcie devices. The return value may be success or failure, and the device
 *           list is not empty when success.
 * @tc.type: FUNC
 */
HWTEST_F(HatsHdfPcieServiceTest, SUB_Driver_HdiPcie_0200, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_pcieInterface);

    std::vector<PcieDeviceInfo> devices;
    int32_t ret = g_pcieInterface->GetPcieDevices(devices);
    if (ret == HDF_SUCCESS) {
        EXPECT_GT(devices.size(), 0);
    }
    printf("get pcie device num[%zu] ret[%d]\n", devices.size(), ret);
}

/**
 * @tc.name: GetPcieDevices002
 * @tc.desc: Get pcie devices and verify the fields. boardId and sysfsPath are always
 *           validated, other fields are only validated when modalias is valid.
 * @tc.type: FUNC
 */
HWTEST_F(HatsHdfPcieServiceTest, SUB_Driver_HdiPcie_0300, Function | MediumTest | Level1)
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
               "modalias[%s]\n",
            device.boardId.c_str(), device.sysfsPath.c_str(), device.vendorId.c_str(),
            device.deviceId.c_str(), device.classCode.c_str(), device.modalias.c_str());
    }
    printf("get pcie devices ret[%d], device num[%zu]\n", ret, devices.size());
}

/**
 * @tc.name: LoadPcieDrivers001
 * @tc.desc: Load pcie drivers with a valid sandbox driver path. The return value may be
 *           success or failure, and the result list is validated when not empty.
 * @tc.type: FUNC
 */
HWTEST_F(HatsHdfPcieServiceTest, SUB_Driver_HdiPcie_0500, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_pcieInterface);

    std::vector<PcieDriverLoadItem> loadRequests;
    PcieDriverLoadItem item;
    item.driverIdentity.boardId = "0000:00:00.0";
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
    printf("load pcie drivers ret[%d]\n", ret);
}

/**
 * @tc.name: LoadPcieDrivers002
 * @tc.desc: Load pcie drivers with an empty request list. The result list is empty.
 * @tc.type: FUNC
 */
HWTEST_F(HatsHdfPcieServiceTest, SUB_Driver_HdiPcie_0600, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_pcieInterface);

    std::vector<PcieDriverLoadItem> loadRequests;
    std::vector<PcieDriverLoadResult> loadResults;
    int32_t ret = g_pcieInterface->LoadPcieDrivers(loadRequests, loadResults);
    if (ret == HDF_SUCCESS) {
        EXPECT_TRUE(loadResults.empty());
    }
    printf("load pcie drivers empty ret[%d], result num[%zu]\n", ret, loadResults.size());
}

/**
 * @tc.name: LoadPcieDrivers003
 * @tc.desc: Load pcie drivers with a driver path outside the sandbox. The load fails.
 * @tc.type: FUNC
 */
HWTEST_F(HatsHdfPcieServiceTest, SUB_Driver_HdiPcie_0700, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_pcieInterface);

    std::vector<PcieDriverLoadItem> loadRequests;
    PcieDriverLoadItem item;
    item.driverIdentity.boardId = "0000:00:00.0";
    item.driverIdentity.driverPath = "/data/local/tmp/pcie_test.ko";
    item.driverIdentity.moduleName = "pcie_test";
    item.options = "";
    loadRequests.push_back(item);

    std::vector<PcieDriverLoadResult> loadResults;
    int32_t ret = g_pcieInterface->LoadPcieDrivers(loadRequests, loadResults);
    EXPECT_NE(HDF_SUCCESS, ret);
    if (!loadResults.empty()) {
        ASSERT_EQ(loadRequests.size(), loadResults.size());
        EXPECT_EQ(item.driverIdentity.boardId, loadResults[0].driverIdentity.boardId);
        printf("load pcie drivers invalid path ret[%d], statusCode[%d]\n",
            ret, loadResults[0].statusCode);
    }
}

/**
 * @tc.name: LoadPcieDrivers004
 * @tc.desc: Load pcie drivers with an invalid driver file name that is not a ko module.
 * @tc.type: FUNC
 */
HWTEST_F(HatsHdfPcieServiceTest, SUB_Driver_HdiPcie_0800, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_pcieInterface);

    std::vector<PcieDriverLoadItem> loadRequests;
    PcieDriverLoadItem item;
    item.driverIdentity.boardId = "0000:00:00.0";
    item.driverIdentity.driverPath = SANDBOX_ROOT + "/pcie_test.txt";
    item.driverIdentity.moduleName = "pcie_test";
    item.options = "";
    loadRequests.push_back(item);

    std::vector<PcieDriverLoadResult> loadResults;
    int32_t ret = g_pcieInterface->LoadPcieDrivers(loadRequests, loadResults);
    EXPECT_NE(HDF_SUCCESS, ret);
    if (!loadResults.empty()) {
        ASSERT_EQ(loadRequests.size(), loadResults.size());
        EXPECT_EQ(item.driverIdentity.boardId, loadResults[0].driverIdentity.boardId);
        printf("load pcie drivers invalid file ret[%d], statusCode[%d]\n",
            ret, loadResults[0].statusCode);
    }
}

/**
 * @tc.name: LoadPcieDrivers005
 * @tc.desc: Load pcie drivers with multiple requests. The return value may be success or
 *           failure, and the result list is validated when not empty.
 * @tc.type: FUNC
 */
HWTEST_F(HatsHdfPcieServiceTest, SUB_Driver_HdiPcie_0900, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_pcieInterface);

    std::vector<PcieDriverLoadItem> loadRequests;
    for (int32_t i = 0; i < 3; i++) {
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
            printf("index[%zu] statusCode[%d], statusMessage[%s]\n",
                i, loadResults[i].statusCode, loadResults[i].statusMessage.c_str());
        }
    }
    printf("load pcie drivers multi ret[%d]\n", ret);
}
}