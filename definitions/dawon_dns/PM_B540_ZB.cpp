// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: DawonDns PM-B540-ZB — hand-graduated parity port.
// IOT smart plug 16A.
//
// Unlike its PM-B530/PM-C140 siblings, the PM-B540 additionally reports
// its own internal die temperature on genDeviceTempCfg (0x0002). z2m wires
// fz.device_temperature + e.device_temperature() and binds genDeviceTempCfg,
// but the auto-generated port carried only kFzOnOff + kFzMetering and so
// dropped the device_temperature channel entirely. Fixed by wiring the
// generic kFzDeviceTemperature converter (0x0002 attr 0x0000 currentTemperature,
// s16 whole °C, no scaling) + the device_temperature expose + 0x0002 bind.
//
// z2m-source: dawon_dns.ts #PM-B540-ZB
//             (fromZigbee: [fz.device_temperature, fz.on_off, fz.metering]).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::dawon_dns {
namespace {
const FzConverter* const kFz_PM_B540_ZB[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzDeviceTemperature,
};
const TzConverter* const kTz_PM_B540_ZB[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_PM_B540_ZB[] = { "PM-B540-ZB" };

constexpr Expose kExposes_PM_B540_ZB[] = {
    {"state",              ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy",             ExposeType::Numeric, Access::State,    "kWh",   nullptr, nullptr, 0},
    {"power",              ExposeType::Numeric, Access::State,    "W",     nullptr, nullptr, 0},
    {"device_temperature", ExposeType::Numeric, Access::State,    "C",     nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_PM_B540_ZB[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0702},  // seMetering
    {1, 0x0002},  // genDeviceTempCfg
};

}  // namespace

extern const PreparedDefinition kDef_PM_B540_ZB{
    .zigbee_models=kModels_PM_B540_ZB, .zigbee_models_count=sizeof(kModels_PM_B540_ZB)/sizeof(kModels_PM_B540_ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PM-B540-ZB", .vendor="DawonDns",
    .meta=nullptr, .exposes=kExposes_PM_B540_ZB, .exposes_count=sizeof(kExposes_PM_B540_ZB)/sizeof(kExposes_PM_B540_ZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_PM_B540_ZB, .from_zigbee_count=sizeof(kFz_PM_B540_ZB)/sizeof(kFz_PM_B540_ZB[0]),
    .to_zigbee=kTz_PM_B540_ZB, .to_zigbee_count=sizeof(kTz_PM_B540_ZB)/sizeof(kTz_PM_B540_ZB[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_PM_B540_ZB, .bindings_count=sizeof(kBindings_PM_B540_ZB)/sizeof(kBindings_PM_B540_ZB[0]),
};

}  // namespace zhc::devices::dawon_dns
