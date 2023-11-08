/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file expected in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for  the specific language governing permissions and
 * limitations under the License.
 */
#include "camera_hdi_uttest_v1_1.h"

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;
int64_t OHOS::Camera::Test::StreamConsumer::g_timestamp[2] = {0};
void CameraHdiTestV1_1Additional::SetUpTestCase(void) {}
void CameraHdiTestV1_1Additional::TearDownTestCase(void) {}
void CameraHdiTestV1_1Additional::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
}

void CameraHdiTestV1_1Additional::TearDown(void)
{
    cameraTest->Close();
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_0100
 * @tc.name   : testPrelaunch001
 * @tc.desc   : Prelaunch cameraId:device/0
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch001, TestSize.Level1)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_0200
 * @tc.name   : testPrelaunch002
 * @tc.desc   : Prelaunch cameraId:device/0
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch002, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_0300
 * @tc.name   : testPrelaunch003
 * @tc.desc   : Prelaunch cameraId:device/0
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch003, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_0400
 * @tc.name   : testPrelaunch004
 * @tc.desc   : Prelaunch cameraId:device/0
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch004, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_0500
 * @tc.name   : testPrelaunch005
 * @tc.desc   : Prelaunch cameraId:{}
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch005, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = {};
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_0600
 * @tc.name   : testPrelaunch006
 * @tc.desc   : Prelaunch cameraId:{}
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch006, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = {};
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_0700
 * @tc.name   : testPrelaunch007
 * @tc.desc   : Prelaunch cameraId:{}
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch007, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = {};
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_0800
 * @tc.name   : testPrelaunch008
 * @tc.desc   : Prelaunch cameraId:{}
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch008, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = {};
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_0900
 * @tc.name   : testPrelaunch009
 * @tc.desc   : Prelaunch cameraId:device/10
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch009, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/10";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1000
 * @tc.name   : testPrelaunch010
 * @tc.desc   : Prelaunch cameraId:device/10
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch010, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "device/10";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1100
 * @tc.name   : testPrelaunch011
 * @tc.desc   : Prelaunch cameraId:device/10
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch011, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "device/10";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1200
 * @tc.name   : testPrelaunch012
 * @tc.desc   : Prelaunch cameraId:device/10
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch012, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "device/10";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1300
 * @tc.name   : testPrelaunch013
 * @tc.desc   : Prelaunch cameraId:ABC123
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch013, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "ABC123";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1400
 * @tc.name   : testPrelaunch014
 * @tc.desc   : Prelaunch cameraId:ABC123
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch014, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "ABC123";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1500
 * @tc.name   : testPrelaunch015
 * @tc.desc   : Prelaunch cameraId:ABC123
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch015, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "ABC123";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1600
 * @tc.name   : testPrelaunch016
 * @tc.desc   : Prelaunch cameraId:ABC123
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch016, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "ABC123";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1700
 * @tc.name   : testPrelaunch017
 * @tc.desc   : Prelaunch cameraId:1
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch017, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1800
 * @tc.name   : testPrelaunch018
 * @tc.desc   : Prelaunch cameraId:1
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch018, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1900
 * @tc.name   : testPrelaunch019
 * @tc.desc   : Prelaunch cameraId:1
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch019, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_2000
 * @tc.name   : testPrelaunch020
 * @tc.desc   : Prelaunch cameraId:1
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch020, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_2100
 * @tc.name   : testPrelaunch021
 * @tc.desc   : Prelaunch cameraId:65535
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch021, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "65535";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_2200
 * @tc.name   : testPrelaunch022
 * @tc.desc   : Prelaunch cameraId:65535
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch022, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "65535";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_2300
 * @tc.name   : testPrelaunch023
 * @tc.desc   : Prelaunch cameraId:65535
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch023, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "65535";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_2400
 * @tc.name   : testPrelaunch024
 * @tc.desc   : Prelaunch cameraId:65535
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch024, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "65535";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_2500
 * @tc.name   : testPrelaunch025
 * @tc.desc   : Prelaunch cameraId:-1
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch025, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "-1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_2600
 * @tc.name   : testPrelaunch026
 * @tc.desc   : Prelaunch cameraId:-1
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch026, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "-1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_2700
 * @tc.name   : testPrelaunch027
 * @tc.desc   : Prelaunch cameraId:-1
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch027, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "-1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_2800
 * @tc.name   : testPrelaunch028
 * @tc.desc   : Prelaunch cameraId:-1
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch028, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "-1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_2900
 * @tc.name   : testPrelaunch029
 * @tc.desc   : Prelaunch cameraId:123
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch029, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "123";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Getdefaultsettings_0100
 * @tc.name   : testGetDefaultSettings001
 * @tc.desc   : GetDefaultSettings
 */
HWTEST_F(CameraHdiTestV1_1Additional, testGetDefaultSettings001, TestSize.Level1)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(true, cameraTest->cameraDevice != nullptr);
        cameraTest->rc = cameraTest->cameraDeviceV1_1->GetDefaultSettings(cameraTest->abilityVec);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_3000
 * @tc.name   : testPrelaunch030
 * @tc.desc   : Prelaunch cameraId:device/100
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch030, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/100";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_3100
 * @tc.name   : testPrelaunch031
 * @tc.desc   : Prelaunch cameraId:device/100
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch031, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "device/100";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_3200
 * @tc.name   : testPrelaunch032
 * @tc.desc   : Prelaunch cameraId:device/100
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch032, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "device/100";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_3300
 * @tc.name   : testPrelaunch033
 * @tc.desc   : Prelaunch cameraId:device/100
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch033, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "device/100";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_3400
 * @tc.name   : testPrelaunch034
 * @tc.desc   : Prelaunch cameraId:100
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch034, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "100";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_3500
 * @tc.name   : testPrelaunch035
 * @tc.desc   : Prelaunch cameraId:100
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch035, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "100";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_3600
 * @tc.name   : testPrelaunch036
 * @tc.desc   : Prelaunch cameraId:100
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch036, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "100";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_3700
 * @tc.name   : testPrelaunch037
 * @tc.desc   : Prelaunch cameraId:100
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch037, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "100";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_3800
 * @tc.name   : testPrelaunch038
 * @tc.desc   : Prelaunch cameraId:255
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch038, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "255";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_3900
 * @tc.name   : testPrelaunch039
 * @tc.desc   : Prelaunch cameraId:255
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch039, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "255";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_4000
 * @tc.name   : testPrelaunch040
 * @tc.desc   : Prelaunch cameraId:255
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch040, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "255";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_4100
 * @tc.name   : testPrelaunch041
 * @tc.desc   : Prelaunch cameraId:255
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch041, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "255";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_4200
 * @tc.name   : testPrelaunch042
 * @tc.desc   : Prelaunch cameraId:ABCD123
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch042, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "ABCD123";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_4300
 * @tc.name   : testPrelaunch043
 * @tc.desc   : Prelaunch cameraId:ABCD123
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch043, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "ABCD123";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_4400
 * @tc.name   : testPrelaunch044
 * @tc.desc   : Prelaunch cameraId:ABCD123
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch044, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "ABCD123";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_4500
 * @tc.name   : testPrelaunch045
 * @tc.desc   : Prelaunch cameraId:ABCD123
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch045, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "ABCD123";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_4600
 * @tc.name   : testPrelaunch046
 * @tc.desc   : Prelaunch cameraId:1A
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch046, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "1A";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_4700
 * @tc.name   : testPrelaunch047
 * @tc.desc   : Prelaunch cameraId:1A
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch047, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "1A";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_4800
 * @tc.name   : testPrelaunch048
 * @tc.desc   : Prelaunch cameraId:1A
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch048, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "1A";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_4900
 * @tc.name   : testPrelaunch049
 * @tc.desc   : Prelaunch cameraId:1A
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch049, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "1A";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_5000
 * @tc.name   : testPrelaunch050
 * @tc.desc   : Prelaunch cameraId:65535A
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch050, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "65535A";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_5100
 * @tc.name   : testPrelaunch051
 * @tc.desc   : Prelaunch cameraId:65535A
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch051, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "65535A";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_5200
 * @tc.name   : testPrelaunch052
 * @tc.desc   : Prelaunch cameraId:65535A
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch052, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "65535A";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_5300
 * @tc.name   : testPrelaunch053
 * @tc.desc   : Prelaunch cameraId:65535A
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch053, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "65535A";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_5400
 * @tc.name   : testPrelaunch054
 * @tc.desc   : Prelaunch cameraId:-1A
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch054, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "-1A";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_5500
 * @tc.name   : testPrelaunch055
 * @tc.desc   : Prelaunch cameraId:-1A
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch055, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "-1A";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_5600
 * @tc.name   : testPrelaunch056
 * @tc.desc   : Prelaunch cameraId:-1A
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch056, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "-1A";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_5700
 * @tc.name   : testPrelaunch057
 * @tc.desc   : Prelaunch cameraId:-1A
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch057, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "-1A";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_5800
 * @tc.name   : testPrelaunch058
 * @tc.desc   : Prelaunch cameraId:deviceA
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch058, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "deviceA";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_5900
 * @tc.name   : testPrelaunch059
 * @tc.desc   : Prelaunch cameraId:deviceA
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch059, TestSize.Level2)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "deviceA";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
    }
}