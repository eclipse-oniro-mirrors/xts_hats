/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "pin_auth_hdi_test.h"
#include "iam_hat_test.h"

using namespace std;
using namespace testing::ext;
using namespace OHOS::UserIam::Common;
using namespace OHOS::HDI::PinAuth;
using namespace OHOS::HDI::PinAuth::V1_0;
using namespace OHOS::HDI::PinAuth::V1_1;
using Property = OHOS::HDI::PinAuth::V1_1::Property;

static ExecutorImpl g_executorImpl(make_shared<OHOS::UserIam::PinAuth::PinAuth>());
static OHOS::Parcel parcel;
int32_t Expectedvalue = 0;
bool g_onResultFlag = false;
bool g_onGetDataFlag = false;
bool g_onGetDataV11Flag = false;

void UserIamPinAuthTestAdditional::SetUpTestCase() {}

void UserIamPinAuthTestAdditional::TearDownTestCase() {}

void UserIamPinAuthTestAdditional::SetUp() {}

void UserIamPinAuthTestAdditional::TearDown() {}

class DummyIExecutorCallback : public OHOS::HDI::PinAuth::IExecutorCallback {
public:
    DummyIExecutorCallback(int32_t onResultResult, int32_t onGetDataResult, int32_t onGetDataV1Result)
        : onResultResult_(onResultResult), onGetDataResult_(onGetDataResult), onGetDataV1Result_(onGetDataV1Result)
    {
    }

    int32_t OnResult(int32_t result, const std::vector<uint8_t> &extraInfo) override
    {
        cout << "result is " << result << " extraInfo len is " << extraInfo.size() << endl;
        g_onResultFlag = true;
        return onResultResult_;
    }

    int32_t OnGetData(uint64_t scheduleId, const std::vector<uint8_t> &algoParameter, uint64_t authSubType) override
    {
        cout << "scheduleId is " << scheduleId << endl;
        cout << " algoParameter len is " << algoParameter.size() << endl;
        cout << " authSubType is " << authSubType << endl;
        g_onGetDataFlag = true;
        return onGetDataResult_;
    }

    int32_t OnGetDataV1_1(uint64_t scheduleId, const std::vector<uint8_t> &algoParameter, uint64_t authSubType,
                          uint32_t algoVersion) override
    {
        cout << "scheduleId is " << scheduleId << endl;
        cout << " authSubType is " << authSubType << endl;
        g_onGetDataV11Flag = true;
        return onGetDataV1Result_;
    }

private:
    int32_t onResultResult_;
    int32_t onGetDataResult_;
    int32_t onGetDataV1Result_;
};
class DummyIExecutorCallbackV1_0 : public IExecutorCallbackV1_0 {
public:
    DummyIExecutorCallbackV1_0(int32_t onResultResult, int32_t onGetDataResult)
        : onResultResult_(onResultResult), onGetDataResult_(onGetDataResult)
    {
    }

    int32_t OnResult(int32_t result, const std::vector<uint8_t> &extraInfo) override
    {
        cout << "result is " << result << " extraInfo len is " << extraInfo.size() << endl;
        g_onResultFlag = true;
        return onResultResult_;
    }

    int32_t OnGetData(uint64_t scheduleId, const std::vector<uint8_t> &algoParameter, uint64_t authSubType) override
    {
        cout << "scheduleId is " << scheduleId << endl;
        cout << " algoParameter len is " << algoParameter.size() << endl;
        cout << " authSubType is " << authSubType << endl;
        g_onGetDataFlag = true;
        return onGetDataResult_;
    }

private:
    int32_t onResultResult_;
    int32_t onGetDataResult_;
};

static void FillTestIExecutorCallback(Parcel &parcel, sptr<OHOS::HDI::PinAuth::IExecutorCallback> &callbackObj)
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
}
static void FillTestIExecutorCallbackV1_0(Parcel &parcel, sptr<IExecutorCallbackV1_0> &callbackObj)
{
    bool isNull = parcel.ReadBool();
    if (isNull) {
        callbackObj = nullptr;
    } else {
        callbackObj = new (std::nothrow) DummyIExecutorCallbackV1_0(parcel.ReadInt32(), parcel.ReadInt32());
        if (callbackObj == nullptr) {
            cout << "callbackObj construct fail" << endl;
        }
    }
}
void FillTestGetPropertyTypeVector(Parcel &parcel, std::vector<GetPropertyType> &types)
{
    std::vector<uint32_t> propertyTypeUint32;
    FillTestUint32Vector(parcel, propertyTypeUint32);
    for (const auto &val : propertyTypeUint32) {
        types.push_back(static_cast<GetPropertyType>(val));
    }

    cout << "success" << endl;
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_GetExecutorInfo_0200
 * @tc.name  testPinAuthTestGetExecutorInfo001
 * @tc.desc  test GetExecutorInfo executorInfo not empty
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestGetExecutorInfo001, Function | MediumTest | Level2)
{
    cout << "start test testPinAuthTestGetExecutorInfo001" << endl;
    ExecutorInfo executorInfo;
    int32_t ret = g_executorImpl.GetExecutorInfo(executorInfo);
    cout << "ret is" << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_GetExecutorInfo_0300
 * @tc.name  testPinAuthTestGetExecutorInfo002
 * @tc.desc  test GetExecutorInfo 1000 times
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestGetExecutorInfo002, Function | MediumTest | Level2)
{
    cout << "start test testPinAuthTestGetExecutorInfo002" << endl;
    ExecutorInfo executorInfo;
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.GetExecutorInfo(executorInfo);
        cout << "ret" << i << "is" << ret << endl;
        EXPECT_NE(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_GetTemplateInfo_0200
 * @tc.name  testPinAuthTestGetTemplateInfo001
 * @tc.desc  test GetTemplateInfo templateId
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestGetTemplateInfo001, Function | MediumTest | Level2)
{
    cout << "start test testPinAuthTestGetTemplateInfo001" << endl;
    uint64_t templateId = 0;
    TemplateInfo templateInfo;
    int32_t ret = g_executorImpl.GetTemplateInfo(templateId, templateInfo);
    cout << "ret is" << ret << endl;
    EXPECT_NE(ret, 0);
    templateId = 0x7FFFFFFFFFFFFFFF;
    ret = g_executorImpl.GetTemplateInfo(templateId, templateInfo);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    templateId = 0xFFFFFFFFFFFFFFFF;
    ret = g_executorImpl.GetTemplateInfo(templateId, templateInfo);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    templateId = 0x10;
    ret = g_executorImpl.GetTemplateInfo(templateId, templateInfo);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_GetTemplateInfo_0600
 * @tc.name  testPinAuthTestGetTemplateInfo005
 * @tc.desc  test GetTemplateInfo 1000 times
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestGetTemplateInfo005, Function | MediumTest | Level2)
{
    cout << "start test testPinAuthTestGetTemplateInfo005" << endl;
    uint64_t templateId = 0x01;
    TemplateInfo templateInfo;
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.GetTemplateInfo(templateId, templateInfo);
        cout << "ret" << i << "is" << ret << endl;
        EXPECT_NE(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_OnRegisterFinish_0200
 * @tc.name  testPinAuthTestOnRegisterFinish001
 * @tc.desc  test OnRegisterFinish empty
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestOnRegisterFinish001, Function | MediumTest | Level1)
{
    cout << "start test testPinAuthTestOnRegisterFinish001" << endl;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> frameworkPublicKey;
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is" << ret << endl;
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
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_OnRegisterFinish_0900
 * @tc.name  testPinAuthTestOnRegisterFinish008
 * @tc.desc  test OnRegisterFinish 1000 times
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestOnRegisterFinish008, Function | MediumTest | Level1)
{
    cout << "start test testPinAuthTestOnRegisterFinish008" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> frameworkPublicKey;
    FillTestUint8Vector(parcel, frameworkPublicKey);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
        cout << "ret" << i << "is" << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_OnSetData_0200
 * @tc.name  testPinAuthTestOnSetData001
 * @tc.desc  test OnSetData scheduleId
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestOnSetData001, Function | MediumTest | Level2)
{
    cout << "start OnSetData" << endl;
    uint64_t scheduleId = 0;
    uint64_t authSubType = parcel.ReadUint64();
    std::vector<uint8_t> data;
    FillTestUint8Vector(parcel, data);
    int32_t ret = g_executorImpl.OnSetData(scheduleId, authSubType, data);
    cout << "ret is" << ret << endl;
    EXPECT_NE(ret, 0);
    scheduleId = 0x7FFFFFFFFFFFFFFF;
    ret = g_executorImpl.OnSetData(scheduleId, authSubType, data);
    cout << "ret is" << ret << endl;
    EXPECT_NE(ret, 0);
    scheduleId = 0xFFFFFFFFFFFFFFFF;
    ret = g_executorImpl.OnSetData(scheduleId, authSubType, data);
    cout << "ret is" << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_OnSetData_0500
 * @tc.name  testPinAuthTestOnSetData004
 * @tc.desc  test OnSetData authSubType
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestOnSetData004, Function | MediumTest | Level2)
{
    cout << "start OnSetData" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    uint64_t authSubType = 0;
    std::vector<uint8_t> data;
    FillTestUint8Vector(parcel, data);
    int32_t ret = g_executorImpl.OnSetData(scheduleId, authSubType, data);
    cout << "ret is" << ret << endl;
    EXPECT_NE(ret, 0);
    authSubType = 0x7FFFFFFFFFFFFFFF;
    ret = g_executorImpl.OnSetData(scheduleId, authSubType, data);
    cout << "ret is" << ret << endl;
    EXPECT_NE(ret, 0);
    authSubType = 0xFFFFFFFFFFFFFFFF;
    ret = g_executorImpl.OnSetData(scheduleId, authSubType, data);
    cout << "ret is" << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_OnSetData_0800
 * @tc.name  testPinAuthTestOnSetData007
 * @tc.desc  test OnSetData data
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestOnSetData007, Function | MediumTest | Level2)
{
    cout << "start OnSetData" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    uint64_t authSubType = parcel.ReadUint64();
    std::vector<uint8_t> data;
    int32_t ret = g_executorImpl.OnSetData(scheduleId, authSubType, data);
    cout << "ret is" << ret << endl;
    EXPECT_NE(ret, 0);
    for (int32_t i = 0; i < 1000; i++) {
        data.push_back(i);
    }
    ret = g_executorImpl.OnSetData(scheduleId, authSubType, data);
    cout << "ret is" << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_OnSetData_1000
 * @tc.name  testPinAuthTestOnSetData009
 * @tc.desc  test OnSetData 1000 times
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestOnSetData009, Function | MediumTest | Level2)
{
    cout << "start OnSetData" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    uint64_t authSubType = parcel.ReadUint64();
    std::vector<uint8_t> data;
    FillTestUint8Vector(parcel, data);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.OnSetData(scheduleId, authSubType, data);
        cout << "ret is" << ret << endl;
        EXPECT_NE(ret, 0);
    }
}

/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_Enroll_0200
 * @tc.name  testPinAuthTestEnroll001
 * @tc.desc  test Enroll scheduleId
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestEnroll001, Function | MediumTest | Level1)
{
    cout << "start test testPinAuthTestEnroll001" << endl;
    uint64_t scheduleId = 0;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallbackV1_0> callbackObj;
    FillTestIExecutorCallbackV1_0(parcel, callbackObj);
    int32_t ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
    cout << "ret is" << ret << endl;
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
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_Enroll_0500
 * @tc.name  testPinAuthTestEnroll004
 * @tc.desc  test Enroll extraInfo empty
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestEnroll004, Function | MediumTest | Level1)
{
    cout << "start test testPinAuthTestEnroll004" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallbackV1_0> callbackObj;
    FillTestIExecutorCallbackV1_0(parcel, callbackObj);
    int32_t ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
    cout << "ret is" << ret << endl;
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
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_Enroll_0800
 * @tc.name  testPinAuthTestEnroll007
 * @tc.desc  test Enroll 1000 times
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestEnroll007, Function | MediumTest | Level1)
{
    cout << "start test testPinAuthTestEnroll007" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallbackV1_0> callbackObj;
    FillTestIExecutorCallbackV1_0(parcel, callbackObj);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
        cout << "ret" << i << "is" << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_Authenticate_0200
 * @tc.name  testPinAuthTestAuthenticate001
 * @tc.desc  test Authenticate scheduleId
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestAuthenticate001, Function | MediumTest | Level1)
{
    cout << "start testPinAuthTestAuthenticate001" << endl;
    uint64_t scheduleId = 0;
    uint64_t templateId = parcel.ReadUint64();
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallbackV1_0> callbackObj;
    FillTestIExecutorCallbackV1_0(parcel, callbackObj);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is" << ret << endl;
    EXPECT_EQ(ret, 0);
    scheduleId = 0xFFFFFFFFFFFFFFFF;
    ret = g_executorImpl.Authenticate(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    scheduleId = 0x7FFFFFFFFFFFFFFF;
    ret = g_executorImpl.Authenticate(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_Authenticate_0500
 * @tc.name  testPinAuthTestAuthenticate004
 * @tc.desc  test Authenticate templateId
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestAuthenticate004, Function | MediumTest | Level1)
{
    cout << "start testPinAuthTestAuthenticate004" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    uint64_t templateId = 0;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallbackV1_0> callbackObj;
    FillTestIExecutorCallbackV1_0(parcel, callbackObj);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is" << ret << endl;
    EXPECT_EQ(ret, 0);
    templateId = 0xFFFFFFFFFFFFFFFF;
    ret = g_executorImpl.Authenticate(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is" << ret << endl;
    EXPECT_EQ(ret, 0);
    templateId = 0x7FFFFFFFFFFFFFFF;
    ret = g_executorImpl.Authenticate(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is" << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_Authenticate_0800
 * @tc.name  testPinAuthTestAuthenticate007
 * @tc.desc  test Authenticate empty
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestAuthenticate007, Function | MediumTest | Level1)
{
    cout << "start testPinAuthTestAuthenticate007" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    uint64_t templateId = parcel.ReadUint64();
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallbackV1_0> callbackObj;
    FillTestIExecutorCallbackV1_0(parcel, callbackObj);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is" << ret << endl;
    EXPECT_EQ(ret, 0);
    ret = g_executorImpl.Authenticate(scheduleId, templateId, extraInfo, nullptr);
    cout << "ret is" << ret << endl;
    EXPECT_NE(ret, 0);
    FillTestUint8Vector(parcel, extraInfo);
    ret = g_executorImpl.Authenticate(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is" << ret << endl;
    EXPECT_EQ(ret, 0);
    ret = g_executorImpl.Authenticate(scheduleId, templateId, extraInfo, nullptr);
    cout << "ret is" << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_Authenticate_1100
 * @tc.name  testPinAuthTestAuthenticate010
 * @tc.desc  test Authenticate 1000 times
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestAuthenticate010, Function | MediumTest | Level1)
{
    cout << "start testPinAuthTestAuthenticate010" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    uint64_t templateId = parcel.ReadUint64();
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallbackV1_0> callbackObj;
    FillTestIExecutorCallbackV1_0(parcel, callbackObj);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.Authenticate(scheduleId, templateId, extraInfo, callbackObj);
        cout << "ret is" << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_Delete_0200
 * @tc.name  testPinAuthTestDelete001
 * @tc.desc  test Delete templateId 0
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestDelete001, Function | MediumTest | Level2)
{
    cout << "start Delete" << endl;
    uint64_t templateId = 0;
    int32_t ret = g_executorImpl.Delete(templateId);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    templateId = 0x7FFFFFFFFFFFFFFF;
    ret = g_executorImpl.Delete(templateId);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    templateId = 0xFFFFFFFFFFFFFFFF;
    ret = g_executorImpl.Delete(templateId);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    templateId = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.Delete(templateId);
        cout << "ret is " << ret << endl;
        EXPECT_NE(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_Cancel_0200
 * @tc.name  testPinAuthTestCancel001
 * @tc.desc  test Cancel scheduleId
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestCancel001, Function | MediumTest | Level1)
{
    cout << "start Cancel" << endl;
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
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_Cancel_0500
 * @tc.name  testPinAuthTestCancel004
 * @tc.desc  test Cancel 1000 times
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestCancel004, Function | MediumTest | Level1)
{
    cout << "start Cancel" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallbackV1_0> callbackObj;
    FillTestIExecutorCallbackV1_0(parcel, callbackObj);

    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
        ret = g_executorImpl.Cancel(scheduleId);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_SendCommand_0200
 * @tc.name  testPinAuthTestSendCommand001
 * @tc.desc  test SendCommand commandId
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestSendCommand001, Function | MediumTest | Level1)
{
    cout << "start SendCommand" << endl;
    int32_t commandId = DEFAULT;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallbackV1_0> callbackObj;
    FillTestIExecutorCallbackV1_0(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    commandId = 0x01;
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
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_SendCommand_0700
 * @tc.name  testPinAuthTestSendCommand006
 * @tc.desc  test SendCommand extraInfo empty
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestSendCommand006, Function | MediumTest | Level1)
{
    cout << "start SendCommand" << endl;
    int32_t commandId = DEFAULT;
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallbackV1_0> callbackObj;
    FillTestIExecutorCallbackV1_0(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    ret = g_executorImpl.SendCommand(commandId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    FillTestUint8Vector(parcel, extraInfo);
    ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    ret = g_executorImpl.SendCommand(commandId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_SendCommand_1000
 * @tc.name  testPinAuthTestSendCommand009
 * @tc.desc  test SendCommand 1000 times
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestSendCommand009, Function | MediumTest | Level1)
{
    cout << "start SendCommand" << endl;
    int32_t commandId = DEFAULT;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallbackV1_0> callbackObj;
    FillTestIExecutorCallbackV1_0(parcel, callbackObj);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_GetProperty_0200
 * @tc.name  testPinAuthTestGetProperty001
 * @tc.desc  test GetProperty
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestGetProperty001, Function | MediumTest | Level2)
{
    cout << "start GetProperty" << endl;
    std::vector<uint64_t> templateIdList;
    std::vector<GetPropertyType> propertyTypes;
    Property property;
    int32_t ret = g_executorImpl.GetProperty(templateIdList, propertyTypes, property);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    FillTestUint64Vector(parcel, templateIdList);
    ret = g_executorImpl.GetProperty(templateIdList, propertyTypes, property);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    propertyTypes.push_back(OHOS::HDI::PinAuth::V1_1::AUTH_SUB_TYPE);
    propertyTypes.push_back(OHOS::HDI::PinAuth::V1_1::LOCKOUT_DURATION);
    propertyTypes.push_back(OHOS::HDI::PinAuth::V1_1::REMAIN_ATTEMPTS);
    ret = g_executorImpl.GetProperty(templateIdList, propertyTypes, property);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_GetProperty_0700
 * @tc.name  testPinAuthTestGetProperty006
 * @tc.desc  test GetProperty 1000 times
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestGetProperty006, Function | MediumTest | Level2)
{
    cout << "start GetProperty" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<GetPropertyType> propertyTypes;
    propertyTypes.push_back(OHOS::HDI::PinAuth::V1_1::AUTH_SUB_TYPE);
    Property property;

    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.GetProperty(templateIdList, propertyTypes, property);

        cout << "ret is " << ret << endl;
        EXPECT_NE(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_EnrollV1_1_0100
 * @tc.name  testPinAuthTestEnrollV1_1001
 * @tc.desc  test EnrollV1_1 scheduleId
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestEnrollV1_1001, Function | MediumTest | Level1)
{
    cout << "start test EnrollV1_1" << endl;
    uint64_t scheduleId = 0;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<OHOS::HDI::PinAuth::IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.EnrollV1_1(scheduleId, extraInfo, callbackObj);
    cout << "ret is" << ret << endl;
    EXPECT_EQ(ret, 0);
    scheduleId = 0x7FFFFFFFFFFFFFFF;
    ret = g_executorImpl.EnrollV1_1(scheduleId, extraInfo, callbackObj);
    cout << "ret is" << ret << endl;
    EXPECT_EQ(ret, 0);
    scheduleId = 0xFFFFFFFFFFFFFFFF;
    ret = g_executorImpl.EnrollV1_1(scheduleId, extraInfo, callbackObj);
    cout << "ret is" << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_EnrollV1_1_0400
 * @tc.name  testPinAuthTestEnrollV1_1004
 * @tc.desc  test EnrollV1_1 empty
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestEnrollV1_1004, Function | MediumTest | Level1)
{
    cout << "start test EnrollV1_1" << endl;
    uint64_t scheduleId = 0;
    std::vector<uint8_t> extraInfo;
    sptr<OHOS::HDI::PinAuth::IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.EnrollV1_1(scheduleId, extraInfo, callbackObj);
    cout << "ret is" << ret << endl;
    EXPECT_EQ(ret, 0);
    ret = g_executorImpl.EnrollV1_1(scheduleId, extraInfo, nullptr);
    cout << "ret is" << ret << endl;
    EXPECT_NE(ret, 0);
    FillTestUint8Vector(parcel, extraInfo);
    ret = g_executorImpl.EnrollV1_1(scheduleId, extraInfo, callbackObj);
    cout << "ret is" << ret << endl;
    EXPECT_EQ(ret, 0);
    ret = g_executorImpl.EnrollV1_1(scheduleId, extraInfo, nullptr);
    cout << "ret is" << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_EnrollV1_1_0700
 * @tc.name  testPinAuthTestEnrollV1_1007
 * @tc.desc  test EnrollV1_1 1000 times
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestEnrollV1_1007, Function | MediumTest | Level1)
{
    cout << "start test EnrollV1_1" << endl;
    uint64_t scheduleId = 0;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<OHOS::HDI::PinAuth::IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.EnrollV1_1(scheduleId, extraInfo, callbackObj);
        cout << "ret is" << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_AuthenticateV1_1_0100
 * @tc.name  testPinAuthTestAuthenticateV1_1001
 * @tc.desc  test AuthenticateV1_1 scheduleId 0
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestAuthenticateV1_1001, Function | MediumTest | Level2)
{
    cout << "start AuthenticateV1_1" << endl;

    uint64_t scheduleId = 0;
    uint64_t templateId = parcel.ReadUint64();
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<OHOS::HDI::PinAuth::IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    scheduleId = 0xFFFFFFFFFFFFFFFF;
    ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    scheduleId = 0x7FFFFFFFFFFFFFFF;
    ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_AuthenticateV1_1_0400
 * @tc.name  testPinAuthTestAuthenticateV1_1004
 * @tc.desc  test AuthenticateV1_1 templateId 0
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestAuthenticateV1_1004, Function | MediumTest | Level2)
{
    cout << "start AuthenticateV1_1" << endl;

    uint64_t scheduleId = parcel.ReadUint64();
    uint64_t templateId = 0;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<OHOS::HDI::PinAuth::IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    templateId = 0xFFFFFFFFFFFFFFFF;
    ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    templateId = 0x7FFFFFFFFFFFFFFF;
    ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_AuthenticateV1_1_0700
 * @tc.name  testPinAuthTestAuthenticateV1_1007
 * @tc.desc  test AuthenticateV1_1 extraInfo empty
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestAuthenticateV1_1007, Function | MediumTest | Level2)
{
    cout << "start AuthenticateV1_1" << endl;

    uint64_t scheduleId = parcel.ReadUint64();
    uint64_t templateId = parcel.ReadUint64();
    std::vector<uint8_t> extraInfo;
    sptr<OHOS::HDI::PinAuth::IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    FillTestUint8Vector(parcel, extraInfo);
    ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
    ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_AuthenticateV1_1_1000
 * @tc.name  testPinAuthTestAuthenticateV1_1010
 * @tc.desc  test AuthenticateV1_1 1000 times
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestAuthenticateV1_1010, Function | MediumTest | Level2)
{
    cout << "start AuthenticateV1_1" << endl;

    uint64_t scheduleId = parcel.ReadUint64();
    uint64_t templateId = parcel.ReadUint64();
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<OHOS::HDI::PinAuth::IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateId, extraInfo, callbackObj);

        cout << "ret is " << ret << endl;
        EXPECT_NE(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_GetExecutorList_0200
 * @tc.name  testPinAuthTestGetExecutorList001
 * @tc.desc  test GetExecutorList 1000 times
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestGetExecutorList001, Function | MediumTest | Level1)
{
    cout << "start GetExecutorList" << endl;
    PinAuthInterfaceService g_pinAuthInterFaceService;
    std::vector<sptr<IExecutorV1_0>> executorList;
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_pinAuthInterFaceService.GetExecutorList(executorList);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_GetExecutorListV1_1_0200
 * @tc.name  testPinAuthTestGetExecutorListV1_1001
 * @tc.desc  test GetExecutorListV1_1 1000 times
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestGetExecutorListV1_1001, Function | MediumTest | Level1)
{
    cout << "start GetExecutorListV1_1" << endl;
    std::vector<sptr<V1_1::IExecutor>> executorList;
    PinAuthInterfaceService pin_Interface;

    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = pin_Interface.GetExecutorListV1_1(executorList);

        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_OnResult_0100
 * @tc.name  testPinAuthTestOnResult001
 * @tc.desc  test OnResult
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestOnResult001, Function | MediumTest | Level1)
{
    cout << "start SendCommand" << endl;
    int32_t commandId = DEFAULT;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallbackV1_0> callbackObj;
    FillTestIExecutorCallbackV1_0(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(g_onResultFlag, true);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_OnGetData_0100
 * @tc.name  testPinAuthTestOnGetData001
 * @tc.desc  test OnGetData
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestOnGetData001, Function | MediumTest | Level1)
{
    cout << "start Authenticate" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    uint64_t templateId = parcel.ReadUint64();
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallbackV1_0> callbackObj;
    FillTestIExecutorCallbackV1_0(parcel, callbackObj);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(g_onGetDataFlag, true);
}
/**
 * @tc.number  SUB_Security_Iam_PinAuth_HDI_OnGetDataV1_1_0100
 * @tc.name  testPinAuthTestOnGetDataV1_1001
 * @tc.desc  test OnGetDataV1_1
 */
HWTEST_F(UserIamPinAuthTestAdditional, testPinAuthTestOnGetDataV1_1001, Function | MediumTest | Level1)
{
    cout << "start AuthenticateV1_1" << endl;

    uint64_t scheduleId = parcel.ReadUint64();
    uint64_t templateId = parcel.ReadUint64();
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<OHOS::HDI::PinAuth::IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateId, extraInfo, callbackObj);

    cout << "ret is " << ret << endl;
    EXPECT_EQ(g_onGetDataV11Flag, true);
}