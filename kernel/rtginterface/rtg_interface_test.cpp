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

#include "gtest/gtest.h"

#define private public
#define protected public
#include <fcntl.h>
#include <securec.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "rtg_interface.h"
#undef private
#undef protected

namespace OHOS {
namespace RME {
using namespace testing;
using namespace testing::ext;

#define RTG_INTERFACE_SO_PATH "/system/lib/librtg_interface.z.so"

enum rtg_type : int {
    VIP = 0,
    TOP_TASK_KEY,
    NORMAL_TASK,
    RTG_TYPE_MAX,
};

class RtgInterfaceTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

namespace {
    const char RTG_SCHED_IPC_MAGIC = 0xAB;
}

#define CMD_ID_SET_RTG \
    _IOWR(RTG_SCHED_IPC_MAGIC, SET_RTG, struct rtg_str_data)

int CreateNewRtgGrp(int prioType, int rtNum)
{
    struct rtg_grp_data grp_data;
    int ret;
    char fileName[] = "/proc/self/sched_rtg_ctrl";
    int fd = open(fileName, O_RDWR);
    if (fd < 0) {
        return fd;
    }
    (void)memset_s(&grp_data, sizeof(struct rtg_grp_data), 0, sizeof(struct rtg_grp_data));
    if ((prioType > 0) && (prioType < RTG_TYPE_MAX)) {
        grp_data.prio_type = prioType;
    }
    if (rtNum > 0) {
        grp_data.rt_cnt = rtNum;
    }
    grp_data.rtg_cmd = CMD_CREATE_RTG_GRP;
    ret = ioctl(fd, CMD_ID_SET_RTG, &grp_data);
    close(fd);
    return ret;
}

void RtgInterfaceTest::SetUpTestCase()
{
}

void RtgInterfaceTest::TearDownTestCase()
{
}

void RtgInterfaceTest::SetUp()
{
    // must enable rtg before use the interface
    bool ret = EnableRtg(true);
    EXPECT_EQ(ret, false);
}

void RtgInterfaceTest::TearDown()
{
    // disable rtg after use the interface
    bool ret = EnableRtg(false);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: RtgInterfaceCreateAndDestroy
 * @tc.desc: Verify the CreateAndDestroy function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceCreateAndDestroy, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(NORMAL_TASK, 0);
    EXPECT_GT(grpId, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceDestroyErrorGroup
 * @tc.desc: Verify Destroy function with error param.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceDestroyErrorGroup, TestSize.Level1)
{
    int ret;
    ret = DestroyRtgGrp(-1);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceAddRtg
 * @tc.desc: Verify Rtg add function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceAddRtg, TestSize.Level1)
{
    int ret;
    int grpId;
    int pid = getpid();
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadToRtg(pid, grpId);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceAddErrorThread
 * @tc.desc: Verify Rtg add function with error pid.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceAddErrorThread, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadToRtg(-1, grpId);
    EXPECT_NE(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceAddErrorGroup
 * @tc.desc: Verify Rtg add function with error groupid.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceAddErrorGroup, TestSize.Level1)
{
    int ret;
    int grpId;
    int pid = getpid();
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadToRtg(pid, (grpId + 1));
    EXPECT_NE(ret, 0);
    ret = AddThreadToRtg(pid, -1);
    EXPECT_NE(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceAddRtgs
 * @tc.desc: Verify Rtg add multiple thread function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceAddRtgs, TestSize.Level1)
{
    int ret;
    int grpId;
    int pid = getpid();
    vector<int> pids = {};
    pids.push_back(pid);
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadsToRtg(pids, grpId);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}


/**
 * @tc.name: RtgInterfaceBeginFrameFreq
 * @tc.desc: Verify rtg frame start function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceBeginFrameFreq, TestSize.Level1)
{
    int ret;
    int grpId;
    int pid = getpid();
    vector<int> pids = {};
    pids.push_back(pid);
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadsToRtg(pids, grpId);
    EXPECT_EQ(ret, 0);
    ret = BeginFrameFreq(0);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}


/**
 * @tc.name: RtgInterfaceBeginFrameFreqWithNoAddThreadtoGrp
 * @tc.desc: Verify rtg frame start function with NoAddThreadtoGrp.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceBeginFrameFreqWithNoAddThreadtoGrp, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = BeginFrameFreq(0);
    EXPECT_NE(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}


/**
 * @tc.name: RtgInterfaceBeginFrameFreqWithErrorParam
 * @tc.desc: Verify rtg frame start function with error Parameter.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceBeginFrameFreqWithErrorParam, TestSize.Level1)
{
    int ret;
    ret = BeginFrameFreq(-1);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceEndFrameFreq
 * @tc.desc: Verify rtg frame end function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceEndFrameFreq, TestSize.Level1)
{
    int ret;
    int grpId;
    int pid = getpid();
    vector<int> pids = {};
    pids.push_back(pid);
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadsToRtg(pids, grpId);
    EXPECT_EQ(ret, 0);
    ret = EndFrameFreq(0);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceEndFrameFreqWithNoAddThreadtoGrp
 * @tc.desc: Verify rtg frame end function with NoAddThreadtoGrp.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceEndFrameFreqWithNoAddThreadtoGrp, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = EndFrameFreq(0);
    EXPECT_NE(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.name: RtgInterfaceEndFrameFreqWithErrorParam
 * @tc.desc: Verify rtg frame end function with error Parameter.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceEndFrameFreqWithErrorParam, TestSize.Level1)
{
    int ret;
    ret = EndFrameFreq(-1);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceEndScene
 * @tc.desc: Verify rtg frame scene end function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceEndScene, TestSize.Level1)
{
    int ret;
    int grpId;
    int pid = getpid();
    vector<int> pids = {};
    pids.push_back(pid);
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadsToRtg(pids, grpId);
    EXPECT_EQ(ret, 0);
    ret = EndScene(grpId);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceEndSceneWithErrorGrp
 * @tc.desc: Verify rtg frame scene end function with error groupid.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceEndSceneWithErrorGrp, TestSize.Level1)
{
    int ret;
    ret = EndScene(-1);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceSetMinUtil
 * @tc.desc: Verify rtg minUtil set function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetMinUtil, TestSize.Level1)
{
    int ret;
    int grpId;
    int pid = getpid();
    vector<int> pids = {};
    pids.push_back(pid);
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadsToRtg(pids, grpId);
    EXPECT_EQ(ret, 0);
    ret = SetMinUtil(0);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceSetMinUtilWithNoAddThreadtoGrp
 * @tc.desc: Verify rtg minUtil set function with NoAddThreadtoGrp.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetMinUtilWithNoAddThreadtoGrp, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = SetMinUtil(0);
    EXPECT_NE(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.name: RtgInterfaceSetMinUtilWithErrorParam
 * @tc.desc: Verify rtg minUtil set function with error Parameter.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetMinUtilWithErrorParam, TestSize.Level1)
{
    int ret;
    ret = SetMinUtil(-1);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceSetMargin
 * @tc.desc: Verify rtg margin set function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetMargin, TestSize.Level1)
{
    int ret;
    int grpId;
    int pid = getpid();
    vector<int> pids = {};
    pids.push_back(pid);
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadsToRtg(pids, grpId);
    EXPECT_EQ(ret, 0);
    ret = SetMargin(0);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceSetMarginWithNoAddThreadtoGrp
 * @tc.desc: Verify rtg margin set function with NoAddThreadtoGrp.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetMarginWithNoAddThreadtoGrp, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = SetMargin(0);
    EXPECT_NE(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.name: RtgInterfaceSetMarginWithErrorParam
 * @tc.desc: Verify rtg margin set function with error parameter.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetMarginWithErrorParam, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = SetMargin(-101);
    EXPECT_NE(ret, 0);
    ret = SetMargin(65536);
    EXPECT_NE(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}


/**
 * @tc.name: RtgInterfaceSetAttr
 * @tc.desc: Verify rtg attr set function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetAttr, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = SetFrameRateAndPrioType(grpId, 60, VIP);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceSetErrorAttr
 * @tc.desc: Verify rtg attr set function with error attr param.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceSetErrorAttr, TestSize.Level1)
{
    int ret;
    int grpId;
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = SetFrameRateAndPrioType(grpId, 0, VIP);
    EXPECT_NE(ret, 0);
    ret = SetFrameRateAndPrioType(grpId, -1, VIP);
    EXPECT_NE(ret, 0);
    ret = SetFrameRateAndPrioType(grpId, 121, VIP);
    EXPECT_NE(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}


/**
 * @tc.name: RtgInterfaceAddDestoryMultipleThreads
 * @tc.desc: Verify rtg multiple add  destory function.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceAddDestoryMultipleThreads, TestSize.Level1)
{
    int ret;
    int pid[3];
    vector<int> threads;
    int grpId;
    for (int i = 0; i < 3; i++) {
        pid[i] = fork();
        ASSERT_TRUE(pid[i] >= 0) << "> parent: fork errno = " << errno;
        if (pid[i] == 0) {
            usleep(50000);
            _Exit(0);
        }
        threads.push_back(pid[i]);
    }
    grpId = CreateNewRtgGrp(NORMAL_TASK, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadsToRtg(threads, grpId);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceAddDestoryVIPGroupMultipleThreads
 * @tc.desc: Verify rtg multiple add destory about VIP group function .
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceAddDestoryVIPGroupMultipleThreads, TestSize.Level1)
{
    int ret;
    int pid[5];
    vector<int> threads;
    int grpId;
    for (int i = 0; i < 5; i++) {
        pid[i] = fork();
        ASSERT_TRUE(pid[i] >= 0) << "> parent: fork errno = " << errno;
        if (pid[i] == 0) {
            usleep(50000);
            _Exit(0);
        }
        threads.push_back(pid[i]);
    }
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadsToRtg(threads, grpId, 36);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: RtgInterfaceAddDestoryRepeatCalls
 * @tc.desc: Verify rtg multiple add destory repeat calls .
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceAddDestoryRepeatCalls, TestSize.Level1)
{
    int ret;
    int pid[4];
    vector<int> threads;
    int grpId;
    for (int i = 0; i < 4; i++) {
        pid[i] = fork();
        ASSERT_TRUE(pid[i] >= 0) << "> parent: fork errno = " << errno;
        if (pid[i] == 0) {
            usleep(50000);
            _Exit(0);
        }
        threads.push_back(pid[i]);
    }
    grpId = CreateNewRtgGrp(VIP, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadsToRtg(threads, grpId);
    EXPECT_EQ(ret, 0);
    ret = AddThreadsToRtg(threads, grpId);
    EXPECT_NE(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: RtgInterfaceAddMultipleThreadsOutOfLimit
 * @tc.desc: Verify rtg multiple add function with out of limit threads.
 * @tc.type: FUNC
 */
HWTEST_F(RtgInterfaceTest, RtgInterfaceAddMultipleThreadsOutOfLimit, TestSize.Level1)
{
    int ret;
    int pid[8];
    vector<int> threads;
    int grpId;
    for (int i = 0; i < 8; i++) {
        pid[i] = fork();
        ASSERT_TRUE(pid[i] >= 0) << "> parent: fork errno = " << errno;
        if (pid[i] == 0) {
            usleep(50000);
            _Exit(0);
        }
        threads.push_back(pid[i]);
    }
    grpId = CreateNewRtgGrp(NORMAL_TASK, 0);
    EXPECT_GT(grpId, 0);
    ret = AddThreadsToRtg(threads, grpId);
    EXPECT_NE(ret, 0);
    ret = DestroyRtgGrp(grpId);
    EXPECT_EQ(ret, 0);
}
} // namespace RME
} // namespace OHOS
