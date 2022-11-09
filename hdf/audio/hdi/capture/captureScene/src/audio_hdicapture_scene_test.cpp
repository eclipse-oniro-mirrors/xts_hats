/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

/**
 * @addtogroup Audio
 * @{
 *
 * @brief Defines audio-related APIs, including custom data types and functions for capture drivers function.
 * accessing a driver adapter, and capturing audios.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file audio_hdi_common.h
 *
 * @brief Declares APIs for operations related to the capturing audio adapter.
 *
 * @since 1.0
 * @version 1.0
 */

#include "audio_hdi_common.h"
#include "audio_hdicapture_scene_test.h"

using namespace std;
using namespace testing::ext;
using namespace OHOS::Audio;

namespace {
class AudioHdiCaptureSceneTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
    int32_t AudioCaptureStart(const string path, struct AudioCapture *capture) const;
};

void AudioHdiCaptureSceneTest::SetUpTestCase(void) {}

void AudioHdiCaptureSceneTest::TearDownTestCase(void) {}
int32_t AudioHdiCaptureSceneTest::AudioCaptureStart(const string path, struct AudioCapture *capture) const
{
    int32_t ret = -1;
    struct AudioSampleAttributes attrs = {};

    ret = InitAttrs(attrs);
    if (ret < 0) {
        return ret;
    }
    FILE *file = fopen(path.c_str(), "wb+");
    if (file == nullptr) {
        return HDF_FAILURE;
    }
    ret = FrameStartCapture(capture, file, attrs);
    (void)fclose(file);
    return ret;
}
void AudioHdiCaptureSceneTest::SetUp(void) {}
void AudioHdiCaptureSceneTest::TearDown(void) {}

/**
* @tc.name   Test AudioCaptureCheckSceneCapability API and check scene's capability
* @tc.number  SUB_Audio_HDI_CaptureCheckSceneCapability_0001
* @tc.desc  Test AudioCaptureCheckSceneCapability interface,return 0 if check scene's capability successful.
*/
HWTEST_F(AudioHdiCaptureSceneTest, SUB_Audio_HDI_CaptureCheckSceneCapability_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    bool supported = false;
    struct AudioSceneDescriptor scenes = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    scenes.scene.id = 0;
    scenes.desc.pins = PIN_IN_MIC;
    ret = capture->scene.CheckSceneCapability(capture, &scenes, &supported);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_TRUE(supported);

    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name   Test checking scene's capability where the scene is not configured in the json.
* @tc.number  SUB_Audio_HDI_CaptureCheckSceneCapability_0002
* @tc.desc  Test AudioCreateCapture interface,return -1 if the scene is not configured in the json.
*/
HWTEST_F(AudioHdiCaptureSceneTest, SUB_Audio_HDI_CaptureCheckSceneCapability_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    bool supported = true;
    struct AudioSceneDescriptor scenes = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    scenes.scene.id = 5;
    scenes.desc.pins = PIN_IN_MIC;
    ret = capture->scene.CheckSceneCapability(capture, &scenes, &supported);
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);

    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name   Test checking scene's capability where the capture is empty
* @tc.number  SUB_Audio_HDI_CaptureCheckSceneCapability_0003
* @tc.desc  Test AudioCreateCapture interface,return -1 if the capture is empty.
*/
HWTEST_F(AudioHdiCaptureSceneTest, SUB_Audio_HDI_CaptureCheckSceneCapability_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    bool supported = true;
    struct AudioSceneDescriptor scenes = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;
    struct AudioCapture *captureNull = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    scenes.scene.id = 0;
    scenes.desc.pins = PIN_IN_MIC;
    ret = capture->scene.CheckSceneCapability(captureNull, &scenes, &supported);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    ret = AudioCaptureStart(AUDIO_CAPTURE_FILE, capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    capture->control.Stop((AudioHandle)capture);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name   Test checking scene's capability where the scene is empty
* @tc.number  SUB_Audio_HDI_CaptureCheckSceneCapability_0004
* @tc.desc  Test AudioCreateCapture interface,return -1 if the scene is empty.
*/
HWTEST_F(AudioHdiCaptureSceneTest, SUB_Audio_HDI_CaptureCheckSceneCapability_0004, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    bool supported = true;
    struct AudioSceneDescriptor *scenes = nullptr;
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = capture->scene.CheckSceneCapability(capture, scenes, &supported);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    ret = AudioCaptureStart(AUDIO_CAPTURE_FILE, capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    capture->control.Stop((AudioHandle)capture);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name   Test checking scene's capability where the parameter supported is empty.
* @tc.number  SUB_Audio_HDI_CaptureCheckSceneCapability_0005
* @tc.desc  Test AudioCreateCapture interface,return -1 if the parameter supported is empty.
*/
HWTEST_F(AudioHdiCaptureSceneTest, SUB_Audio_HDI_CaptureCheckSceneCapability_0005, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioSceneDescriptor scenes = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    scenes.scene.id = 0;
    scenes.desc.pins = PIN_IN_MIC;
    ret = capture->scene.CheckSceneCapability(capture, &scenes, nullptr);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCaptureSelectScene API via legal input
* @tc.number  SUB_Audio_HDI_AudioCaptureSelectScene_0001
* @tc.desc  Test AudioCaptureSelectScene interface,return 0 if select capture's scene successful.
*/
HWTEST_F(AudioHdiCaptureSceneTest, SUB_Audio_HDI_AudioCaptureSelectScene_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioSceneDescriptor scenes = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    scenes.scene.id = 0;
    scenes.desc.pins = PIN_IN_MIC;
    ret = capture->scene.SelectScene(capture, &scenes);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCaptureSelectScene API after capture start.
* @tc.number  SUB_Audio_HDI_AudioCaptureSelectScene_0002
* @tc.desc  Test AudioCaptureSelectScene, return 0 if select capture's scene successful after capture start.
*/
HWTEST_F(AudioHdiCaptureSceneTest, SUB_Audio_HDI_AudioCaptureSelectScene_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioSceneDescriptor scenes = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = AudioCaptureStart(AUDIO_CAPTURE_FILE, capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    scenes.scene.id = 0;
    scenes.desc.pins = PIN_IN_MIC;
    ret = capture->scene.SelectScene(capture, &scenes);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    capture->control.Stop((AudioHandle)capture);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCaptureSelectScene API where the parameter handle is empty.
* @tc.number  SUB_Audio_HDI_AudioCaptureSelectScene_0003
* @tc.desc  Test AudioCaptureSelectScene, return -1 if the parameter handle is empty.
*/
HWTEST_F(AudioHdiCaptureSceneTest, SUB_Audio_HDI_AudioCaptureSelectScene_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioSceneDescriptor scenes = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;
    struct AudioCapture *captureNull = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    scenes.scene.id = 0;
    scenes.desc.pins = PIN_IN_MIC;
    ret = capture->scene.SelectScene(captureNull, &scenes);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCaptureSelectScene API where the parameter scene is empty.
* @tc.number  SUB_Audio_HDI_AudioCaptureSelectScene_0004
* @tc.desc  Test AudioCaptureSelectScene, return -1 if the parameter scene is empty.
*/
HWTEST_F(AudioHdiCaptureSceneTest, SUB_Audio_HDI_AudioCaptureSelectScene_0004, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioSceneDescriptor *scenes = nullptr;
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = capture->scene.SelectScene(capture, scenes);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCaptureSelectScene API where the scene is not configured in the json.
* @tc.number  SUB_Audio_HDI_AudioCaptureSelectScene_0005
* @tc.desc  Test AudioCaptureSelectScene, return -1 if the scene is not configured in the json.
*/
HWTEST_F(AudioHdiCaptureSceneTest, SUB_Audio_HDI_AudioCaptureSelectScene_0005, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioSceneDescriptor scenes = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    scenes.scene.id = 5;
    scenes.desc.pins = PIN_OUT_HDMI;
    ret = capture->scene.SelectScene(capture, &scenes);
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);

    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
}
