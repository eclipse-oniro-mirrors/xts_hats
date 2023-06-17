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
#include <servmgr_hdi.h>
#include "v1_1/iwlan_interface.h"
#include "wlan_callback_impl.h"
#include "wlan_impl.h"

#define HDF_LOG_TAG service_manager_test
using namespace testing::ext;

namespace HdiDirectTest {
const int32_t DEFAULT_COMBO_SIZE = 6;
const int32_t WLAN_MAX_NUM_STA_WITH_AP = 4;
const int32_t WLAN_FREQ_MAX_NUM = 35;
const int32_t WLAN_TX_POWER = 160;
const char *WLAN_SERVICE_NAME = "wlan_interface_service";

class HdfWifiDirectTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

static struct IWlanInterface *g_wlanObj = nullptr;
struct IWlanCallback *g_wlanCallbackObj = nullptr;
void HdfWifiDirectTest::SetUpTestCase()
{
    g_wlanObj = IWlanInterfaceGetInstance(WLAN_SERVICE_NAME, true);
    g_wlanCallbackObj = WlanCallbackServiceGet();
    ASSERT_TRUE(g_wlanObj != nullptr);
    ASSERT_TRUE(g_wlanCallbackObj != nullptr);
}

void HdfWifiDirectTest::TearDownTestCase()
{
    IWlanInterfaceReleaseInstance(WLAN_SERVICE_NAME, g_wlanObj, true);
    WlanCallbackServiceRelease(g_wlanCallbackObj);
}

void HdfWifiDirectTest::SetUp()
{
}

void HdfWifiDirectTest::TearDown()
{
}

/**
 * @tc.name: GetSupportFeatureTest_001
 * @tc.desc: Wifi hdi get support feature function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, GetSupportFeatureTest_001, Function | MediumTest | Level2)
{
    uint8_t supType[PROTOCOL_80211_IFTYPE_NUM + 1] = {0};
    uint32_t supTypeLenInvalid = 6;

    int32_t rc = g_wlanObj->GetSupportFeature(g_wlanObj, nullptr, &supTypeLenInvalid);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetSupportFeature(g_wlanObj, supType, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetSupportFeature(g_wlanObj, supType, &supTypeLenInvalid);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: GetSupportComboTest_002
 * @tc.desc: Wifi hdi get support combo function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, GetSupportComboTest_002, Function | MediumTest | Level2)
{
    uint64_t combo[DEFAULT_COMBO_SIZE] = {0};

    int32_t rc = g_wlanObj->GetSupportCombo(g_wlanObj, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetSupportCombo(g_wlanObj, combo);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: CreateFeatureTest_003
 * @tc.desc: Wifi hdi create feature function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, CreateFeatureTest_003, Function | MediumTest | Level2)
{
    struct HdfFeatureInfo ifeature;
    const int32_t wlanType = PROTOCOL_80211_IFTYPE_AP;

    (void)memset_s(&ifeature, sizeof(struct HdfFeatureInfo), 0, sizeof(struct HdfFeatureInfo));
    int32_t rc = g_wlanObj->CreateFeature(g_wlanObj, wlanType, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->CreateFeature(g_wlanObj, wlanType, &ifeature);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: DestroyFeatureTest_004
 * @tc.desc: Wifi hdi destroy feature function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, DestroyFeatureTest_004, Function | MediumTest | Level2)
{
    struct HdfFeatureInfo ifeature;
    string ifName = "wlan0";

    (void)memset_s(&ifeature, sizeof(struct HdfFeatureInfo), 0, sizeof(struct HdfFeatureInfo));
    int32_t rc = g_wlanObj->DestroyFeature(g_wlanObj, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = nullptr;
    rc = g_wlanObj->DestroyFeature(g_wlanObj, &ifeature);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = const_cast<char*>(ifName.c_str());
    rc = g_wlanObj->DestroyFeature(g_wlanObj, &ifeature);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: GetAssociatedStasTest_005
 * @tc.desc: Wifi hdi get associated stas function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, GetAssociatedStasTest_005, Function | MediumTest | Level2)
{
    struct HdfFeatureInfo ifeature;
    struct HdfStaInfo staInfo[WLAN_MAX_NUM_STA_WITH_AP] = {{0}};
    uint32_t staInfoLen = WLAN_MAX_NUM_STA_WITH_AP;
    uint32_t num = 0;
    string ifName = "wlan0";

    (void)memset_s(&ifeature, sizeof(struct HdfFeatureInfo), 0, sizeof(struct HdfFeatureInfo));
    int32_t rc = g_wlanObj->GetAssociatedStas(g_wlanObj, nullptr, staInfo, &staInfoLen, &num);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = nullptr;
    rc = g_wlanObj->GetAssociatedStas(g_wlanObj, &ifeature, staInfo, &staInfoLen, &num);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = const_cast<char*>(ifName.c_str());
    rc = g_wlanObj->GetAssociatedStas(g_wlanObj, &ifeature, nullptr, &staInfoLen, &num);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetAssociatedStas(g_wlanObj, &ifeature, staInfo, nullptr, &num);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetAssociatedStas(g_wlanObj, &ifeature, staInfo, &staInfoLen, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetAssociatedStas(g_wlanObj, &ifeature, staInfo, &staInfoLen, &num);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: GetChipIdTest_006
 * @tc.desc: Wifi hdi get chip id function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, GetChipIdTest_006, Function | MediumTest | Level2)
{
    struct HdfFeatureInfo ifeature;
    uint8_t chipId = 0;
    string ifName = "wlan0";

    (void)memset_s(&ifeature, sizeof(struct HdfFeatureInfo), 0, sizeof(struct HdfFeatureInfo));
    int32_t rc = g_wlanObj->GetChipId(g_wlanObj, nullptr, &chipId);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = nullptr;
    rc = g_wlanObj->GetChipId(g_wlanObj, &ifeature, &chipId);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = const_cast<char*>(ifName.c_str());
    rc = g_wlanObj->GetChipId(g_wlanObj, &ifeature, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetChipId(g_wlanObj, &ifeature, &chipId);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: GetDeviceMacAddressTest_007
 * @tc.desc: Wifi hdi get device mac addr function test on STA feature
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, GetDeviceMacAddressTest_007, Function | MediumTest | Level2)
{
    struct HdfFeatureInfo ifeature;
    uint8_t mac[ETH_ADDR_LEN] = {0};
    uint32_t macLen = ETH_ADDR_LEN;
    string ifName = "wlan0";

    (void)memset_s(&ifeature, sizeof(struct HdfFeatureInfo), 0, sizeof(struct HdfFeatureInfo));
    int32_t rc = g_wlanObj->GetDeviceMacAddress(g_wlanObj, nullptr, mac, &macLen, ETH_ADDR_LEN);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = nullptr;
    rc = g_wlanObj->GetDeviceMacAddress(g_wlanObj, &ifeature, mac, &macLen, ETH_ADDR_LEN);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = const_cast<char*>(ifName.c_str());
    rc = g_wlanObj->GetDeviceMacAddress(g_wlanObj, &ifeature, nullptr, &macLen, ETH_ADDR_LEN);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetDeviceMacAddress(g_wlanObj, &ifeature, mac, nullptr, ETH_ADDR_LEN);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetDeviceMacAddress(g_wlanObj, &ifeature, mac, &macLen, ETH_ADDR_LEN);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: GetFeatureByIfNameTest_008
 * @tc.desc: Wifi hdi get feature by ifname function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, GetFeatureByIfNameTest_008, Function | MediumTest | Level2)
{
    struct HdfFeatureInfo ifeature;
    const char *ifName = "wlan0";

    (void)memset_s(&ifeature, sizeof(struct HdfFeatureInfo), 0, sizeof(struct HdfFeatureInfo));
    int32_t rc = g_wlanObj->GetFeatureByIfName(g_wlanObj, nullptr, &ifeature);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetFeatureByIfName(g_wlanObj, ifName, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetFeatureByIfName(g_wlanObj, ifName, &ifeature);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: GetFeatureTypeTest_009
 * @tc.desc: Wifi hdi get feature type function test on STA feature
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, GetFeatureTypeTest_009, Function | MediumTest | Level2)
{
    struct HdfFeatureInfo ifeature;
    int32_t featureType;

    (void)memset_s(&ifeature, sizeof(struct HdfFeatureInfo), 0, sizeof(struct HdfFeatureInfo));
    int32_t rc = g_wlanObj->GetFeatureType(g_wlanObj, nullptr, &featureType);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetFeatureType(g_wlanObj, &ifeature, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetFeatureType(g_wlanObj, &ifeature, &featureType);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: GetFreqsWithBandTest_010
 * @tc.desc: Wifi hdi get freqs function test on STA feature
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, GetFreqsWithBandTest_010, Function | MediumTest | Level2)
{
    struct HdfFeatureInfo ifeature;
    struct HdfWifiInfo wifiInfo;
    int32_t freq[WLAN_FREQ_MAX_NUM] = {0};
    uint32_t freqLen = WLAN_FREQ_MAX_NUM;
    wifiInfo.band = IEEE80211_BAND_2GHZ;
    wifiInfo.size = WLAN_FREQ_MAX_NUM;
    string ifName = "wlan0";

    (void)memset_s(&ifeature, sizeof(struct HdfFeatureInfo), 0, sizeof(struct HdfFeatureInfo));
    int32_t rc = g_wlanObj->GetFreqsWithBand(g_wlanObj, nullptr, &wifiInfo, freq, &freqLen);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = nullptr;
    rc = g_wlanObj->GetFreqsWithBand(g_wlanObj, &ifeature, &wifiInfo, freq, &freqLen);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = const_cast<char*>(ifName.c_str());
    rc = g_wlanObj->GetFreqsWithBand(g_wlanObj, &ifeature, nullptr, freq, &freqLen);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetFreqsWithBand(g_wlanObj, &ifeature, &wifiInfo, nullptr, &freqLen);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetFreqsWithBand(g_wlanObj, &ifeature, &wifiInfo, freq, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetFreqsWithBand(g_wlanObj, &ifeature, &wifiInfo, freq, &freqLen);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: GetChipIdTest_011
 * @tc.desc: Wifi hdi get chip id function test on STA feature
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, GetChipIdTest_011, Function | MediumTest | Level2)
{
    uint8_t chipId = 0;
    uint32_t num = 0;
    char ifNames[IFNAMSIZ] = {0};

    int32_t rc = g_wlanObj->GetIfNamesByChipId(g_wlanObj, chipId, nullptr, IFNAMSIZ, &num);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetIfNamesByChipId(g_wlanObj, chipId, ifNames, IFNAMSIZ, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetIfNamesByChipId(g_wlanObj, chipId, ifNames, IFNAMSIZ, &num);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: GetNetworkIfaceNameTest_012
 * @tc.desc: Wifi hdi get network interface name function test on STA feature
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, GetNetworkIfaceNameTest_012, Function | MediumTest | Level2)
{
    struct HdfFeatureInfo ifeature;
    char ifNames[IFNAMSIZ] = {0};
    string ifName = "wlan0";

    (void)memset_s(&ifeature, sizeof(struct HdfFeatureInfo), 0, sizeof(struct HdfFeatureInfo));
    int32_t rc = g_wlanObj->GetNetworkIfaceName(g_wlanObj, nullptr, ifNames, IFNAMSIZ);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = nullptr;
    rc = g_wlanObj->GetNetworkIfaceName(g_wlanObj, &ifeature, ifNames, IFNAMSIZ);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = const_cast<char*>(ifName.c_str());
    rc = g_wlanObj->GetNetworkIfaceName(g_wlanObj, &ifeature, nullptr, IFNAMSIZ);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetNetworkIfaceName(g_wlanObj, &ifeature, ifNames, IFNAMSIZ);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: RegisterEventCallbackTest_013
 * @tc.desc: Wifi hdi register event call back function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, RegisterEventCallbackTest_013, Function | MediumTest | Level2)
{
    const char *ifName = "wlan0";

    int32_t rc = g_wlanObj->RegisterEventCallback(g_wlanObj, nullptr, ifName);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->RegisterEventCallback(g_wlanObj, g_wlanCallbackObj, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->RegisterEventCallback(g_wlanObj, g_wlanCallbackObj, ifName);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: ResetDriverTest_014
 * @tc.desc: Wifi hdi reset driver function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, ResetDriverTest_014, Function | MediumTest | Level2)
{
    const char *ifName = "wlan0";
    uint8_t chipId = 0;

    int32_t rc = g_wlanObj->ResetDriver(g_wlanObj, chipId, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->ResetDriver(g_wlanObj, chipId, ifName);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: StartScanTest_015
 * @tc.desc: Wifi hdi start scan function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, StartScanTest_015, Function | MediumTest | Level2)
{
    struct HdfFeatureInfo ifeature;
    struct HdfWifiScan scan = {0};
    string ifName = "wlan0";

    (void)memset_s(&ifeature, sizeof(struct HdfFeatureInfo), 0, sizeof(struct HdfFeatureInfo));
    int32_t rc = g_wlanObj->StartScan(g_wlanObj, nullptr, &scan);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = nullptr;
    rc = g_wlanObj->StartScan(g_wlanObj, &ifeature, &scan);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = const_cast<char*>(ifName.c_str());
    rc = g_wlanObj->StartScan(g_wlanObj, &ifeature, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->StartScan(g_wlanObj, &ifeature, &scan);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: UnregisterEventCallbackTest_016
 * @tc.desc: Wifi hdi unreister event call back function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, UnregisterEventCallbackTest_016, Function | MediumTest | Level2)
{
    const char *ifName = "wlan0";

    int32_t rc = g_wlanObj->UnregisterEventCallback(g_wlanObj, nullptr, ifName);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->UnregisterEventCallback(g_wlanObj, g_wlanCallbackObj, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->UnregisterEventCallback(g_wlanObj, g_wlanCallbackObj, ifName);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: SetCountryCodeTest_017
 * @tc.desc: Wifi hdi set country code function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, SetCountryCodeTest_017, Function | MediumTest | Level2)
{
    const char *code = "CN";
    struct HdfFeatureInfo ifeature;
    const char *codeDigital = "99";
    uint32_t size = 2;
    string ifName = "wlan0";

    (void)memset_s(&ifeature, sizeof(struct HdfFeatureInfo), 0, sizeof(struct HdfFeatureInfo));
    int32_t rc = g_wlanObj->SetCountryCode(g_wlanObj, nullptr, codeDigital, size);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = nullptr;
    rc = g_wlanObj->SetCountryCode(g_wlanObj, &ifeature, code, size);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = const_cast<char*>(ifName.c_str());
    rc = g_wlanObj->SetCountryCode(g_wlanObj, &ifeature, nullptr, size);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->SetCountryCode(g_wlanObj, &ifeature, code, size);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: SetMacAddressTest_018
 * @tc.desc: Wifi hdi set mac addr function test on STA feature
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, SetMacAddressTest_018, Function | MediumTest | Level2)
{
    uint8_t mac[ETH_ADDR_LEN] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};
    struct HdfFeatureInfo ifeature;
    uint32_t macLen = ETH_ADDR_LEN;
    string ifName = "wlan0";

    (void)memset_s(&ifeature, sizeof(struct HdfFeatureInfo), 0, sizeof(struct HdfFeatureInfo));
    int32_t rc = g_wlanObj->SetMacAddress(g_wlanObj, nullptr, mac, macLen);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = nullptr;
    rc = g_wlanObj->SetMacAddress(g_wlanObj, &ifeature, mac, macLen);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = const_cast<char*>(ifName.c_str());
    rc = g_wlanObj->SetMacAddress(g_wlanObj, &ifeature, nullptr, macLen);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->SetMacAddress(g_wlanObj, &ifeature, mac, macLen);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: SetScanningMacAddressTest_019
 * @tc.desc: Wifi hdi set scanning mac addr function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, SetScanningMacAddressTest_019, Function | MediumTest | Level2)
{
    struct HdfFeatureInfo ifeature;
    uint8_t scanMac[ETH_ADDR_LEN] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};
    uint32_t macLen = ETH_ADDR_LEN;
    string ifName = "wlan0";

    (void)memset_s(&ifeature, sizeof(struct HdfFeatureInfo), 0, sizeof(struct HdfFeatureInfo));
    int32_t rc = g_wlanObj->SetScanningMacAddress(g_wlanObj, nullptr, scanMac, macLen);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = nullptr;
    rc = g_wlanObj->SetScanningMacAddress(g_wlanObj, &ifeature, scanMac, macLen);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = const_cast<char*>(ifName.c_str());
    rc = g_wlanObj->SetScanningMacAddress(g_wlanObj, &ifeature, nullptr, macLen);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->SetScanningMacAddress(g_wlanObj, &ifeature, scanMac, macLen);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: SetTxPowerTest_020
 * @tc.desc: Wifi hdi set tx power function test on STA feature
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, SetTxPowerTest_020, Function | MediumTest | Level2)
{
    struct HdfFeatureInfo ifeature;
    int32_t power = WLAN_TX_POWER;
    string ifName = "wlan0";

    (void)memset_s(&ifeature, sizeof(struct HdfFeatureInfo), 0, sizeof(struct HdfFeatureInfo));
    int32_t rc = g_wlanObj->SetTxPower(g_wlanObj, nullptr, power);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = nullptr;
    rc = g_wlanObj->SetTxPower(g_wlanObj, &ifeature, power);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = const_cast<char*>(ifName.c_str());
    rc = g_wlanObj->SetTxPower(g_wlanObj, &ifeature, power);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: GetNetdevInfoTest_021
 * @tc.desc: Wifi hdi get netdev info function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, GetNetdevInfoTest_021, Function | MediumTest | Level2)
{
    int32_t rc;
    struct HdfNetDeviceInfoResult netDeviceInfoResult;

    (void)memset_s(
        &netDeviceInfoResult, sizeof(struct HdfNetDeviceInfoResult), 0, sizeof(struct HdfNetDeviceInfoResult));
    rc = g_wlanObj->GetNetDevInfo(g_wlanObj, (struct HdfNetDeviceInfoResult *)&netDeviceInfoResult);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetNetDevInfo(g_wlanObj, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.name: GetPowerModeTest_022
 * @tc.desc: Wifi hdi get power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, GetPowerModeTest_022, Function | MediumTest | Level2)
{
    struct HdfFeatureInfo ifeature;
    uint8_t mode = 0;
    string ifName = "wlan0";

    (void)memset_s(&ifeature, sizeof(struct HdfFeatureInfo), 0, sizeof(struct HdfFeatureInfo));
    int32_t rc = g_wlanObj->GetPowerMode(g_wlanObj, nullptr, &mode);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = nullptr;
    rc = g_wlanObj->GetPowerMode(g_wlanObj, &ifeature, &mode);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = const_cast<char*>(ifName.c_str());
    rc = g_wlanObj->GetPowerMode(g_wlanObj, &ifeature, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetPowerMode(g_wlanObj, &ifeature, &mode);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: SetPowerModeTest_023
 * @tc.desc: Wifi hdi set power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, SetPowerModeTest_023, Function | MediumTest | Level2)
{
    struct HdfFeatureInfo ifeature;
    uint8_t mode = WIFI_POWER_MODE_SLEEPING;
    string ifName = "wlan0";

    (void)memset_s(&ifeature, sizeof(struct HdfFeatureInfo), 0, sizeof(struct HdfFeatureInfo));
    int32_t rc = g_wlanObj->SetPowerMode(g_wlanObj, nullptr, mode);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = nullptr;
    rc = g_wlanObj->SetPowerMode(g_wlanObj, &ifeature, mode);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    ifeature.ifName = const_cast<char*>(ifName.c_str());
    rc = g_wlanObj->SetPowerMode(g_wlanObj, &ifeature, mode);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: SetProjectionScreenParam_024
 * @tc.desc: Wifi hdi set paramters to optimize projectino screen function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, SetProjectionScreenParam_024, Function | MediumTest | Level2)
{
    const char *ifName = "wlan0";
    int32_t rc;
    struct ProjectionScreenCmdParam param;

    (void)memset_s(&param, sizeof(struct ProjectionScreenCmdParam), 0, sizeof(struct ProjectionScreenCmdParam));
    rc = g_wlanObj->SetProjectionScreenParam(g_wlanObj, nullptr, &param);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->SetProjectionScreenParam(g_wlanObj, ifName, nullptr);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->SetProjectionScreenParam(g_wlanObj, ifName, &param);
    ASSERT_EQ(rc, HDF_FAILURE);
}

/**
 * @tc.name: GetStaInfo_025
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(HdfWifiDirectTest, GetStaInfo_025, Function | MediumTest | Level2)
{
    const char *ifName = "wlan0";
    int32_t rc;
    struct WifiStationInfo info;
    uint8_t mac[ETH_ADDR_LEN] = {0};

    (void)memset_s(&info, sizeof(struct WifiStationInfo), 0, sizeof(struct WifiStationInfo));
    rc = g_wlanObj->GetStaInfo(g_wlanObj, nullptr, &info, mac, ETH_ADDR_LEN);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetStaInfo(g_wlanObj, ifName, nullptr, mac, ETH_ADDR_LEN);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetStaInfo(g_wlanObj, ifName, &info, nullptr, ETH_ADDR_LEN);
    ASSERT_EQ(rc, HDF_ERR_INVALID_PARAM);
    rc = g_wlanObj->GetStaInfo(g_wlanObj, ifName, &info, mac, ETH_ADDR_LEN);
    ASSERT_EQ(rc, HDF_FAILURE);
}
};
