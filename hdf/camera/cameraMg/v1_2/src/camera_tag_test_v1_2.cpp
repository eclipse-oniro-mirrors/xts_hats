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
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "camera_tag_test_v1_2.h"

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;
constexpr uint32_t ITEM_CAPACITY = 100;
constexpr uint32_t DATA_CAPACITY = 2000;
void CameraTagTestV1_2::SetUpTestCase(void) {}
void CameraTagTestV1_2::TearDownTestCase(void) {}
void CameraTagTestV1_2::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init(); // assert inside
    cameraTest->Open(DEVICE_0); // assert inside
}

void CameraTagTestV1_2::TearDown(void)
{
    cameraTest->Close();
}

bool IsTagValueExistsU8(std::shared_ptr<CameraMetadata> ability, uint32_t tag, uint8_t value)
{
    common_metadata_header_t* data = ability->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, tag, &entry);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(entry.count, 0);
    for (int i = 0; i < entry.count; i++) {
        if (entry.data.u8[i] == value) {
            return true;
        }
    }
    return false;
}

void invalidParmTestU8(int tag, uint8_t value)
{
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    uint8_t valueTest = value;
    meta->addEntry(tag, &valueTest, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    int rc = HDI::Camera::V1_0::NO_ERROR;
    EXPECT_EQ(rc, HDI::Camera::V1_0::NO_ERROR);
}

void invalidParmTestI32(int tag, int32_t value)
{
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    int32_t valueTest = value;
    meta->addEntry(tag, &valueTest, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    int rc = HDI::Camera::V1_0::NO_ERROR;
    EXPECT_EQ(rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: SUB_Driver_Camera_Modes_0020
 * @tc.desc: OHOS_ABILITY_CAMERA_MODES
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_2, SUB_Driver_Camera_Modes_0020, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_CAMERA_MODES, &entry);
    EXPECT_EQ(ret, 0);
    CAMERA_LOGI("get OHOS_ABILITY_CAMERA_MODES success");
    EXPECT_EQ(META_TYPE_BYTE, entry.data_type);
    for (int i = 0; i < entry.count; i++) {
        if (entry.data.u8[i] == HDI::Camera::V1_0::OperationMode::NORMAL) {
            CAMERA_LOGI("OperationMode::NORMAL found!");
        } else if (entry.data.u8[i] == OHOS::HDI::Camera::V1_1::PORTRAIT) {
            CAMERA_LOGI("OperationMode::PORTRAIT found!");
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_Modes_0030
 * @tc.desc: OHOS_ABILITY_SCENE_PORTRAIT_EFFECT_TYPES, OHOS_CONTROL_PORTRAIT_EFFECT_TYPE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_2, SUB_Driver_Camera_Modes_0030, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_SCENE_PORTRAIT_EFFECT_TYPES, &entry);
    if (ret != 0) {
        bool portraitFlag = IsTagValueExistsU8(cameraTest->ability, OHOS_ABILITY_CAMERA_MODES,
            OHOS::HDI::Camera::V1_1::PORTRAIT);
        EXPECT_EQ(portraitFlag, false);
        CAMERA_LOGI("OHOS::HDI::Camera::V1_1::PORTRAIT found!");
        return;
    }
    printf("OHOS_ABILITY_SCENE_PORTRAIT_EFFECT_TYPES value count is %d\n", entry.count);
    if (entry.count == 0) {
        CAMERA_LOGI("OHOS_ABILITY_SCENE_PORTRAIT_EFFECT_TYPES value count is 0");
        return;
    } else {
        for (size_t i = 0; i < entry.count; i++)
        {
            std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
            printf("OHOS_ABILITY_SCENE_PORTRAIT_EFFECT_TYPES : %d\n", entry.data.u8[i]);
            uint8_t value = entry.data.u8[i];
            meta->addEntry(OHOS_CONTROL_PORTRAIT_EFFECT_TYPE, &value, 1);
            std::vector<uint8_t> metaVec;
            MetadataUtils::ConvertMetadataToVec(meta, metaVec);
            cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
            EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
            CAMERA_LOGI("addEntry for OHOS_CONTROL_PORTRAIT_EFFECT_TYPE success!");
            cameraTest->TakePhotoWithTags(meta);
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_Modes_0040
 * @tc.desc: OHOS_ABILITY_SCENE_FILTER_TYPES, OHOS_CONTROL_FILTER_TYPE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_2, SUB_Driver_Camera_Modes_0040, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_SCENE_FILTER_TYPES, &entry);
    if (ret != 0) {
        CAMERA_LOGI("OHOS_ABILITY_SCENE_FILTER_TYPES not found");
        return;
    }
    CAMERA_LOGI("OHOS_ABILITY_SCENE_FILTER_TYPES found");
    printf("OHOS_ABILITY_SCENE_FILTER_TYPES value count is %d\n", entry.count);
    if (entry.count == 0) {
        CAMERA_LOGI("OHOS_ABILITY_SCENE_FILTER_TYPES value count is 0");
        return;
    } else {
        for (size_t i = 0; i < entry.count; i++) {
            std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
            printf("OHOS_ABILITY_SCENE_FILTER_VALUES : %d\n", entry.data.u8[i]);
            uint8_t value = entry.data.u8[i];
            meta->addEntry(OHOS_CONTROL_FILTER_TYPE, &value, 1);
            std::vector<uint8_t> metaVec;
            MetadataUtils::ConvertMetadataToVec(meta, metaVec);
            cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
            EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
            CAMERA_LOGI("addEntry for OHOS_CONTROL_FILTER_TYPE success!");
            cameraTest->TakePhotoWithTags(meta);
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_Modes_0060
 * @tc.desc: OHOS_ABILITY_BEAUTY_AUTO_VALUES, OHOS_CONTROL_BEAUTY_AUTO_VALUE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_2, SUB_Driver_Camera_Modes_0060, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    camera_metadata_item_t entry;
    bool beautyAutoFlag = IsTagValueExistsU8(cameraTest->ability, OHOS_ABILITY_SCENE_BEAUTY_TYPES,
        OHOS_CAMERA_BEAUTY_TYPE_AUTO);
    if (!beautyAutoFlag) {
        CAMERA_LOGE("OHOS_CAMERA_BEAUTY_TYPE_AUTO not found");
        return;
    }

    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_BEAUTY_AUTO_VALUES, &entry);
    if (ret != 0) {
        CAMERA_LOGI("OHOS_ABILITY_BEAUTY_AUTO_VALUES not found");
        return;
    }
    printf("OHOS_ABILITY_BEAUTY_AUTO_VALUES value count is %d\n", entry.count);
    if (entry.count == 0) {
        CAMERA_LOGI("OHOS_ABILITY_BEAUTY_AUTO_VALUES value count is 0");
        return;
    } else {
        for (size_t i = 0; i < entry.count; i++) {
            std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
            printf("OHOS_ABILITY_BEAUTY_AUTO_VALUES : %d\n", entry.data.u8[i]);
            uint8_t value = entry.data.u8[i];
            meta->addEntry(OHOS_CONTROL_BEAUTY_AUTO_VALUE, &value, 1);
            std::vector<uint8_t> metaVec;
            MetadataUtils::ConvertMetadataToVec(meta, metaVec);
            cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
            EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
            CAMERA_LOGI("addEntry for OHOS_CONTROL_BEAUTY_AUTO_VALUE success!");
            cameraTest->TakePhotoWithTags(meta);
        }
    }
}