// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: CustomDevicesDiy FanBee — auto-generated.
// Fan with valve
// z2m-source: custom_devices_diy.ts #FanBee.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {
const FzConverter* const kFz_FanBee[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzFanMode,
};
const TzConverter* const kTz_FanBee[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_FanBee[] = { "FanBee1", "Fanbox2" };

}  // namespace


// z2m: exposes=[e.fan().withState().withSpeed()] -> on/off + numeric fan_mode (0..6).
// fan_mode mapping: 0=off,1=low,2=medium,3=high,4=on,5=auto,6=smart (genFanCtrl 0x0202 attr 0).
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"fan_mode", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0202},
};

extern const PreparedDefinition kDef_FanBee{
    .zigbee_models=kModels_FanBee, .zigbee_models_count=sizeof(kModels_FanBee)/sizeof(kModels_FanBee[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="FanBee", .vendor="CustomDevicesDiy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_FanBee, .from_zigbee_count=sizeof(kFz_FanBee)/sizeof(kFz_FanBee[0]),
    .to_zigbee=kTz_FanBee, .to_zigbee_count=sizeof(kTz_FanBee)/sizeof(kTz_FanBee[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::custom_devices_diy
