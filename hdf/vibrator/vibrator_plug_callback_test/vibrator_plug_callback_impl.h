/*
 * Copyright (c) 2023-2025 Huawei Device Co., Ltd.
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

#ifndef OHOS_HDI_SENSOR_V2_0_SENSORCALLBACKIMPL_H
#define OHOS_HDI_SENSOR_V2_0_SENSORCALLBACKIMPL_H

#include <hdf_base.h>
#include "v2_0/ivibrator_plug_callback.h"
#include "hdf_log.h"

namespace OHOS {
namespace HDI {
namespace Vibrator {
namespace V2_0 {
class VibratorPlugCallbackImpl : public IVibratorPlugCallback {
public:
    virtual ~VibratorPlugCallbackImpl() {}

    int32_t OnVibratorPlugEvent(const HdfVibratorPlugInfo& hdfVibratorPlugInfo) override
    {
        printf("OnVibratorPlugEvent deviceId[%d]deviceName[%s]vibratorCnt[%d]status[%d]",
               hdfVibratorPlugInfo.deviceId, hdfVibratorPlugInfo.deviceName.c_str(),
               hdfVibratorPlugInfo.vibratorCnt, hdfVibratorPlugInfo.status);
        return HDF_SUCCESS;
    }
};
} // V2_0
} // Vibrator
} // HDI
} // OHOS

#endif // OHOS_HDI_SENSOR_V1_1_SENSORCALLBACKIMPL_H
