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

#include <benchmark/benchmark.h>
#include <string>
#include <vector>

#include <cmath>
#include <cstdio>
#include <gtest/gtest.h>
#include <securec.h>
#include "hdf_base.h"
#include "osal_time.h"
#include "v1_0/ilight_interface.h"
#include "light_type.h"

using namespace OHOS::HDI::Light::V1_0;
using namespace testing::ext;
using namespace std;

namespace {
    constexpr uint32_t g_sleepTime = 3;
    constexpr int32_t g_minLightId = HDF_LIGHT_ID_BATTERY;
    constexpr int32_t g_maxLightId = HDF_LIGHT_ID_ATTENTION;
    constexpr int32_t g_onTime = 500;
    constexpr int32_t g_offTime = 500;
    sptr<ILightInterface> g_lightInterface = nullptr;

class lightBenchmarkTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state);
    void TearDown(const ::benchmark::State &state);
};

void lightBenchmarkTest::SetUp(const ::benchmark::State &state)
{
    g_lightInterface = ILightInterface::Get();
}

void lightBenchmarkTest::TearDown(const ::benchmark::State &state)
{
}

/**
  * @tc.name: CheckLightInstanceIsEmpty
  * @tc.desc: Creat a light instance. The instance is not empty.
  * @tc.type: FUNC
  */
BENCHMARK_F(lightBenchmarkTest, CheckLightInstanceIsEmpty)(benchmark::State &st)
{
    for (auto _ : st) {
        ASSERT_NE(nullptr, g_lightInterface);
    }
}

BENCHMARK_REGISTER_F(lightBenchmarkTest, CheckLightInstanceIsEmpty)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetLightInfo001
  * @tc.desc: Get light info.
  * @tc.type: FUNC
  */
BENCHMARK_F(lightBenchmarkTest, GetLightInfo001)(benchmark::State &st)
{
    ASSERT_NE(nullptr, g_lightInterface);

    std::vector<HdfLightInfo> info;
    for (auto _ : st) {
        int32_t ret = g_lightInterface->GetLightInfo(info);
        EXPECT_EQ(0, ret);
    }
    printf("get light list num[%zu]\n\r", info.size());

    for (auto iter : info)
    {
        EXPECT_GE(iter.lightId, g_minLightId);
        EXPECT_LE(iter.lightId, g_maxLightId);
    }
}

BENCHMARK_REGISTER_F(lightBenchmarkTest, GetLightInfo001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: TurnOnLight001
  * @tc.desc: TurnOnLight.
  * @tc.type: FUNC
  */
BENCHMARK_F(lightBenchmarkTest, TurnOnLight001)(benchmark::State &st)
{
    ASSERT_NE(nullptr, g_lightInterface);

    std::vector<HdfLightInfo> info;
    int32_t ret = g_lightInterface->GetLightInfo(info);
    EXPECT_EQ(HDF_SUCCESS, ret);
    printf("get light list num[%zu]\n\r", info.size());

    for (auto iter : info)
    {
        EXPECT_GE(iter.lightId, g_minLightId);
        EXPECT_LE(iter.lightId, g_maxLightId);

        HdfLightEffect effect;
        effect.lightBrightness = 0x00800000;
        effect.flashEffect.flashMode = HDF_LIGHT_FLASH_NONE;
        for (auto _ : st) {
            int32_t ret = g_lightInterface->TurnOnLight(iter.lightId, effect);
            EXPECT_EQ(HDF_SUCCESS, ret);
        }
        OsalSleep(g_sleepTime);
        ret = g_lightInterface->TurnOffLight(iter.lightId);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

BENCHMARK_REGISTER_F(lightBenchmarkTest, TurnOnLight001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: TurnOnLight003
  * @tc.desc: TurnOnLight.
  * @tc.type: FUNC
  */
BENCHMARK_F(lightBenchmarkTest, TurnOnLight003)(benchmark::State &st)
{
    ASSERT_NE(nullptr, g_lightInterface);

    std::vector<HdfLightInfo> info;
    int32_t ret = g_lightInterface->GetLightInfo(info);
    EXPECT_EQ(0, ret);
    printf("get light list num[%zu]\n\r", info.size());

    for (auto iter : info)
    {
        EXPECT_GE(iter.lightId, g_minLightId);
        EXPECT_LE(iter.lightId, g_maxLightId);

        HdfLightEffect effect;
        effect.lightBrightness = 0x00800000;
        effect.flashEffect.flashMode = HDF_LIGHT_FLASH_TIMED;
        effect.flashEffect.onTime = g_onTime;
        effect.flashEffect.offTime = g_offTime;
        for (auto _ : st) {
            int32_t ret = g_lightInterface->TurnOnLight(iter.lightId, effect);
            EXPECT_EQ(HDF_SUCCESS, ret);
        }
        OsalSleep(g_sleepTime);
        ret = g_lightInterface->TurnOffLight(iter.lightId);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

BENCHMARK_REGISTER_F(lightBenchmarkTest, TurnOnLight003)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();
}

BENCHMARK_MAIN();