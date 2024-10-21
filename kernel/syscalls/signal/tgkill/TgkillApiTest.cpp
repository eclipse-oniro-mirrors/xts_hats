/*
 * Copyright (C) 2024 HiHope Open Source Organization.
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

#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/syscall.h>
#include <sys/tgkill.h>
using namespace testing::ext;

class HatsTgkillTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsTgkillTest::SetUp()
{
}
void HatsTgkillTest::TearDown()
{
}
void HatsTgkillTest::SetUpTestCase()
{
}
void HatsTgkillTest::TearDownTestCase()
{
}

void Handle(int sig)
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TGKILL_0100
 * @tc.name   : TgkillSendSignalToExistingProcessSuccess_0001
 * @tc.desc   : tgkill send signal to existing process success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsTgkillTest, TgkillSendSignalToExistingProcessSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    pid_t tid;
    struct sigaction sa;

    sa.sa_handler = &Handle;
    ret = sigemptyset(&sa.sa_mask);
    EXPECT_EQ(ret, 0);
    sa.sa_flags = 0;
    ret = sigaction(SIGUSR1, &sa, NULL);
    EXPECT_EQ(ret, 0);
    tid = gettid();
    ret = tgkill(getpid(), tid, SIGUSR1);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TGKILL_0200
 * @tc.name   : TgkillSendSignalToNonExistingProcessFail_0002
 * @tc.desc   : tgkill send signal to nonExisting process fail, return -1, and set errno.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsTgkillTest, TgkillSendSignalToNonExistingProcessFail_0002, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = tgkill(-1, -1, SIGUSR1);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}
