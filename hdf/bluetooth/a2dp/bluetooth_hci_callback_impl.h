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

#ifndef OHOS_HDI_BLUETOOTH_HCI_V1_0_IHCICALLBACKIMPL_H
#define OHOS_HDI_BLUETOOTH_HCI_V1_0_IHCICALLBACKIMPL_H

#include <hdf_base.h>
#include "v1_0/ibluetooth_audio_callback.h"
using OHOS::HDI::Bluetooth::A2dp::V1_0::IBluetoothAudioCallback;
namespace OHOS {
namespace HDI {
namespace Bluetooth {
namespace A2dp {
namespace V1_0 {
class SeClientCallback : public IBluetoothAudioCallback {
public:
    SeClientCallback()
    {}
    virtual ~SeClientCallback()
    {}
   
    int32_t StartRender() override;
    
    int32_t SuspendRender() override;

    int32_t StopRender() override;

};
} // V1_0
} // A2dp
} // Bluetooth
} // HDI
} // OHOS

#endif // OHOS_HDI_BLUETOOTH_HCI_V1_0_IHCICALLBACKIMPL_H
