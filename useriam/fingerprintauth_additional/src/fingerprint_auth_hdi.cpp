/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#include "fingerprint_auth_hdi_test.h"

#include <hdf_base.h>

#include "iam_hat_test.h"

#define LOG_LABEL OHOS::UserIam::Common::LABEL_FINGERPRINT_AUTH_IMPL

using namespace std;
using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::UserIam::Common;
using namespace OHOS::HDI::FingerprintAuth;

static AllInOneExecutorImpl g_executorImpl;
static Parcel parcel;
bool g_onResultFlag = false;
bool g_onTipFlag = false;
bool g_onSaCommandsFlag = false;

void UserIamFingerprintAuthTestAdditional::SetUpTestCase()
{
}

void UserIamFingerprintAuthTestAdditional::TearDownTestCase()
{
}

void UserIamFingerprintAuthTestAdditional::SetUp()
{
}

void UserIamFingerprintAuthTestAdditional::TearDown()
{
}

class DummyIExecutorCallback : public IExecutorCallback {
public:
    DummyIExecutorCallback(int32_t result, int32_t tip, int32_t message) : result_(result), tip_(tip), message_(message)
    {
    }

    int32_t OnResult(int32_t result, const std::vector<uint8_t> &extraInfo) override
    {
        g_onResultFlag = true;
        cout << "result is " << result << " extraInfo len is " << extraInfo.size() << endl;
        return result_;
    }

    int32_t OnTip(int32_t tip, const std::vector<uint8_t> &extraInfo) override
    {
        cout << "tip is " << tip << " extraInfo len is " << extraInfo.size() << endl;
        g_onTipFlag = true;
        return tip_;
    }

    int32_t OnMessage(int32_t destRole, const std::vector<uint8_t> &msg) override
    {
        cout << "destRole is " << destRole << " msg len is " << msg.size() << endl;
        return message_;
    }

private:
    int32_t result_;
    int32_t tip_;
    int32_t message_;
};

class DummyISaCommandCallback : public ISaCommandCallback {
public:
    explicit DummyISaCommandCallback(int32_t result) : result_(result)
    {
    }

    int32_t OnSaCommands(const std::vector<SaCommand> &commands) override
    {
        g_onSaCommandsFlag = true;
        return result_;
    }

private:
    int32_t result_;
};

static void FillTestIExecutorCallback(Parcel &parcel, sptr<IExecutorCallback> &callbackObj)
{
    bool isNull = parcel.ReadBool();
    if (isNull) {
        callbackObj = nullptr;
    } else {
        callbackObj =
            new (std::nothrow) DummyIExecutorCallback(parcel.ReadInt32(), parcel.ReadInt32(), parcel.ReadInt32());
        if (callbackObj == nullptr) {
            cout << "callbackObj construct fail" << endl;
        }
    }
    cout << "success" << endl;
}

void FillTestProperty(Parcel &parcel, Property &property)
{
    property.authSubType = parcel.ReadUint64();
    property.lockoutDuration = parcel.ReadInt32();
    property.remainAttempts = parcel.ReadInt32();
    FillTestString(parcel, property.enrollmentProgress);
    FillTestString(parcel, property.sensorInfo);

    cout << "success" << endl;
}

void FillTestISaCommandCallback(Parcel &parcel, sptr<ISaCommandCallback> &callbackObj)
{
    bool isNull = parcel.ReadBool();
    if (isNull) {
        callbackObj = nullptr;
    } else {
        callbackObj = new (std::nothrow) DummyISaCommandCallback(parcel.ReadInt32());
        if (callbackObj == nullptr) {
            cout << "callbackObj construct fail" << endl;
        }
    }
    cout << "success" << endl;
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetExecutorInfo_0200
 * @tc.name  testFingerprintAuthTestGetExecutorInfo001
 * @tc.desc  test GetExecutorInfo executorInfo not empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetExecutorInfo001,
    Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetExecutorInfo001" << endl;
    ExecutorInfo executorInfo;
    int32_t ret = g_executorImpl.GetExecutorInfo(executorInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetExecutorInfo_0300
 * @tc.name  testFingerprintAuthTestGetExecutorInfo002
 * @tc.desc  test GetExecutorInfo 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetExecutorInfo002,
    Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetExecutorInfo002" << endl;
    ExecutorInfo executorInfo;
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.GetExecutorInfo(executorInfo);
        cout << "ret " << i << " is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_OnRegisterFinish_0200
 * @tc.name  testFingerprintAuthTestOnRegisterFinish001
 * @tc.desc  test OnRegisterFinish templateIdList
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestOnRegisterFinish001,
    Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestOnRegisterFinish001" << endl;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> frameworkPublicKey;
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    FillTestUint64Vector(parcel, templateIdList);
    ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    FillTestUint8Vector(parcel, frameworkPublicKey);
    ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    FillTestUint8Vector(parcel, extraInfo);
    ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_OnRegisterFinish_0900
 * @tc.name  testFingerprintAuthTestOnRegisterFinish008
 * @tc.desc  test OnRegisterFinish 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestOnRegisterFinish008,
    Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestOnRegisterFinish008" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> frameworkPublicKey;
    FillTestUint8Vector(parcel, frameworkPublicKey);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
        cout << "ret " << i << " is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Enroll_0200
 * @tc.name  testFingerprintAuthTestEnroll001
 * @tc.desc  test Enroll scheduleId
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestEnroll001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestEnroll001" << endl;
    uint64_t scheduleId = 0;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    scheduleId = 0x7FFFFFFFFFFFFFFF;
    ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    scheduleId = 0xFFFFFFFFFFFFFFFF;
    ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Enroll_0500
 * @tc.name  testFingerprintAuthTestEnroll004
 * @tc.desc  test Enroll extraInfo empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestEnroll004, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestEnroll004" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    ret = g_executorImpl.Enroll(scheduleId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    FillTestUint8Vector(parcel, extraInfo);
    ret = g_executorImpl.Enroll(scheduleId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Enroll_0800
 * @tc.name  testFingerprintAuthTestEnroll007
 * @tc.desc  test Enroll 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestEnroll007, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestEnroll007" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
        cout << "ret " << i << " is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Identify_0200
 * @tc.name  testFingerprintAuthTestIdentify001
 * @tc.desc  test Identify scheduleId
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestIdentify001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestIdentify001" << endl;
    uint64_t scheduleId = 0;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Identify(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    scheduleId = 0x7FFFFFFFFFFFFFFF;
    ret = g_executorImpl.Identify(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    scheduleId = 0xFFFFFFFFFFFFFFFF;
    ret = g_executorImpl.Identify(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Identify_0500
 * @tc.name  testFingerprintAuthTestIdentify004
 * @tc.desc  test Identify extraInfo
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestIdentify004, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestIdentify004" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Identify(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    ret = g_executorImpl.Identify(scheduleId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    FillTestUint8Vector(parcel, extraInfo);
    ret = g_executorImpl.Identify(scheduleId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Identify_0800
 * @tc.name  testFingerprintAuthTestIdentify007
 * @tc.desc  test Identify 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestIdentify007, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestIdentify007" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.Identify(scheduleId, extraInfo, callbackObj);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Delete_0200
 * @tc.name  testFingerprintAuthTestDelete001
 * @tc.desc  test Delete templateIdList empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestDelete001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestDelete001" << endl;
    std::vector<uint64_t> templateIdList;
    int32_t ret = g_executorImpl.Delete(templateIdList);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Delete_0300
 * @tc.name  testFingerprintAuthTestDelete002
 * @tc.desc  test Delete 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestDelete002, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestDelete002" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.Delete(templateIdList);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Cancel_0200
 * @tc.name  testFingerprintAuthTestCancel001
 * @tc.desc  test Cancel scheduleId
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestCancel001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestCancel001" << endl;
    uint64_t scheduleId = 0;
    int32_t ret = g_executorImpl.Cancel(scheduleId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    scheduleId = 0x7FFFFFFFFFFFFFFF;
    ret = g_executorImpl.Cancel(scheduleId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    scheduleId = 0xFFFFFFFFFFFFFFFF;
    ret = g_executorImpl.Cancel(scheduleId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Cancel_0500
 * @tc.name  testFingerprintAuthTestCancel004
 * @tc.desc  test Cancel 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestCancel004, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestCancel004" << endl;
    uint64_t scheduleId = 0;
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.Cancel(scheduleId);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SendCommand_0200
 * @tc.name  testFingerprintAuthTestSendCommand001
 * @tc.desc  test SendCommand commandId enum
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSendCommand001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestSendCommand001" << endl;
    int32_t commandId = DriverCommandId::LOCK_TEMPLATE;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    commandId = DriverCommandId::UNLOCK_TEMPLATE;
    ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    commandId = DriverCommandId::VENDOR_COMMAND_BEGIN;
    ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    commandId = 0;
    ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    commandId = 0x7FFFFFFF;
    ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    commandId = -0x01;
    ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    commandId = -0x7FFFFFFF;
    ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SendCommand_0900
 * @tc.name  testFingerprintAuthTestSendCommand008
 * @tc.desc  test SendCommand extraInfo
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSendCommand008, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestSendCommand008" << endl;
    int32_t commandId = DriverCommandId::LOCK_TEMPLATE;
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    ret = g_executorImpl.SendCommand(commandId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    FillTestUint8Vector(parcel, extraInfo);
    ret = g_executorImpl.SendCommand(commandId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SendCommand_1200
 * @tc.name  testFingerprintAuthTestSendCommand011
 * @tc.desc  test SendCommand 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSendCommand011, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestSendCommand011" << endl;
    int32_t commandId = DriverCommandId::LOCK_TEMPLATE;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Authenticate_0100
 * @tc.name  testFingerprintAuthTestAuthenticate001
 * @tc.desc  test Authenticate scheduleId 0
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticate001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticate001" << endl;
    uint64_t scheduleId = 0;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, true, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    scheduleId = 0x7FFFFFFFFFFFFFFF;
    ret = g_executorImpl.Authenticate(scheduleId, templateIdList, true, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    scheduleId = 0xFFFFFFFFFFFFFFFF;
    ret = g_executorImpl.Authenticate(scheduleId, templateIdList, true, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Authenticate_0400
 * @tc.name  testFingerprintAuthTestAuthenticate004
 * @tc.desc  test Authenticate endAfterFirstFail true
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticate004, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticate004" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, true, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    ret = g_executorImpl.Authenticate(scheduleId, templateIdList, false, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Authenticate_0600
 * @tc.name  testFingerprintAuthTestAuthenticate006
 * @tc.desc  test Authenticate empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticate006, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticate006" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, true, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    ret = g_executorImpl.Authenticate(scheduleId, templateIdList, true, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    FillTestUint64Vector(parcel, templateIdList);
    ret = g_executorImpl.Authenticate(scheduleId, templateIdList, true, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    ret = g_executorImpl.Authenticate(scheduleId, templateIdList, true, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    FillTestUint8Vector(parcel, extraInfo);
    ret = g_executorImpl.Authenticate(scheduleId, templateIdList, true, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    ret = g_executorImpl.Authenticate(scheduleId, templateIdList, true, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Authenticate_1300
 * @tc.name  testFingerprintAuthTestAuthenticate013
 * @tc.desc  test Authenticate 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticate013, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticate013" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.Authenticate(scheduleId, templateIdList, true, extraInfo, callbackObj);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetProperty_0200
 * @tc.name  testFingerprintAuthTestGetProperty001
 * @tc.desc  test GetProperty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetProperty001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetProperty001" << endl;
    std::vector<uint64_t> templateIdList;
    std::vector<int32_t> propertyTypes;
    Property property;
    int32_t ret = g_executorImpl.GetProperty(templateIdList, propertyTypes, property);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    FillTestUint64Vector(parcel, templateIdList);
    ret = g_executorImpl.GetProperty(templateIdList, propertyTypes, property);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    propertyTypes.push_back(GetPropertyType::AUTH_SUB_TYPE);
    propertyTypes.push_back(GetPropertyType::LOCKOUT_DURATION);
    propertyTypes.push_back(GetPropertyType::REMAIN_ATTEMPTS);
    propertyTypes.push_back(GetPropertyType::ENROLL_PROGRESS);
    propertyTypes.push_back(GetPropertyType::SENSOR_INFO);
    ret = g_executorImpl.GetProperty(templateIdList, propertyTypes, property);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    FillTestProperty(parcel, property);
    ret = g_executorImpl.GetProperty(templateIdList, propertyTypes, property);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetProperty_1000
 * @tc.name  testFingerprintAuthTestGetProperty009
 * @tc.desc  test GetProperty 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetProperty009, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetProperty009" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<int32_t> propertyTypes;
    FillTestInt32Vector(parcel, propertyTypes);
    Property property;
    FillTestProperty(parcel, property);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.GetProperty(templateIdList, propertyTypes, property);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SetCachedTemplates_0200
 * @tc.name  testFingerprintAuthTestSetCachedTemplates001
 * @tc.desc  test SetCachedTemplates templateIdList empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSetCachedTemplates001,
    Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestSetCachedTemplates001" << endl;
    std::vector<uint64_t> templateIdList;

    int32_t ret = g_executorImpl.SetCachedTemplates(templateIdList);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SetCachedTemplates_0300
 * @tc.name  testFingerprintAuthTestSetCachedTemplates002
 * @tc.desc  test SetCachedTemplates 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSetCachedTemplates002,
    Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestSetCachedTemplates002" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);

    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.SetCachedTemplates(templateIdList);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number
 * SUB_Security_Iam_FingerprintAuth_HDI_RegisterSaCommandCallback_0200
 * @tc.name  testFingerprintAuthTestRegisterSaCommandCallback001
 * @tc.desc  test SetCachedTemplates callbackObj null
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestRegisterSaCommandCallback001,
    Function | MediumTest | Level2)
{
    cout << "start test testFingerprintAuthTestRegisterSaCommandCallback001" << endl;
    sptr<ISaCommandCallback> callbackObj = nullptr;
    FillTestISaCommandCallback(parcel, callbackObj);

    int32_t ret = g_executorImpl.RegisterSaCommandCallback(nullptr);

    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number
 * SUB_Security_Iam_FingerprintAuth_HDI_RegisterSaCommandCallback_0300
 * @tc.name  testFingerprintAuthTestRegisterSaCommandCallback002
 * @tc.desc  test SetCachedTemplates 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestRegisterSaCommandCallback002,
    Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestRegisterSaCommandCallback002" << endl;
    sptr<ISaCommandCallback> callbackObj = nullptr;
    FillTestISaCommandCallback(parcel, callbackObj);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.RegisterSaCommandCallback(callbackObj);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SendMessage_0100
 * @tc.name  testFingerprintAuthTestSendMessage001
 * @tc.desc  test SetCachedTemplates 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSendMessage001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestSendMessage001" << endl;

    uint64_t scheduleId = 0;
    int32_t srcRole = 0;
    std::vector<uint8_t> msg;
    msg.resize(5);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.SendMessage(scheduleId, srcRole, msg);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetExecutorList_0200
 * @tc.name  testFingerprintAuthTestGetExecutorList001
 * @tc.desc  test GetExecutorList 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetExecutorList001,
    Function | MediumTest | Level1)
{
    cout << "start test GetExecutorList" << endl;
    FingerprintAuthInterfaceService fingerprint_Interface;
    std::vector<sptr<IAllInOneExecutor>> executorList;
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = fingerprint_Interface.GetExecutorList(executorList);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_OnResult_0100
 * @tc.name  testFingerprintAuthTestOnResult001
 * @tc.desc  test OnResult Callback
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestOnResult001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestOnResult001" << endl;
    uint8_t commandId = parcel.ReadUint8();
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(g_onResultFlag, true);
}