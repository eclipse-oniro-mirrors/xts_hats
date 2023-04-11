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
#include "death_test.h"
#include <securec.h>
#include "hdf_base.h"
#include "hdf_log.h"
#include "gtest/gtest.h"
#include "v1_0/display_buffer_type.h"
#include "v1_0/display_composer_type.h"
#include "v1_0/include/idisplay_buffer.h"

namespace OHOS {
namespace HDI {
namespace Display {
namespace TEST {
using namespace OHOS::HDI::Display::Composer::V1_0;
using namespace OHOS::HDI::Display::Buffer::V1_0;
using namespace testing::ext;

void DeathTest::SetUp()
{
}

void DeathTest::TearDown()
{
}

void BufferDiedRecipient::OnRemoteDied(const OHOS::wptr<OHOS::IRemoteObject>& remote)
{
    if (remote == nullptr) {
        return;
    }
    HDF_LOGI("allocator service is dead");
}

HWTEST_F(DeathTest, SUB_DriverSystem_DisplayBufferDeath_0010, TestSize.Level1)
{
    displayBuffer_ = IDisplayBuffer::Get();
    sptr<IRemoteObject::DeathRecipient> recipient = new BufferDiedRecipient();
    auto ret = displayBuffer_->AddDeathRecipient(recipient);
    EXPECT_EQ(ret, true);
    system("killall allocator_host");
}

HWTEST_F(DeathTest, SUB_DriverSystem_DisplayBufferDeath_0020, TestSize.Level1)
{
    displayBuffer_ = IDisplayBuffer::Get();
    sptr<IRemoteObject::DeathRecipient> recipient = new BufferDiedRecipient();
    auto ret = displayBuffer_->AddDeathRecipient(recipient);
    EXPECT_EQ(ret, true);

    ret = displayBuffer_->RemoveDeathRecipient();
    EXPECT_EQ(ret, true);
    system("killall allocator_host");
}
} // OHOS
} // HDI
} // DISPLAY
} // TEST
