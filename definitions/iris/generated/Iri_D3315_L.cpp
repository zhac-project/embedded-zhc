// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Iris 3315-L — hand-rewritten 2026-04-28q.
// Water sensor.
// z2m-source: iris.ts #3315-L.
//   fromZigbee: [fz.temperature, fz.ias_water_leak_alarm_1, fz.battery]
//   exposes:    [water_leak, battery_low, tamper, temperature, battery]
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::iris {
namespace {
const FzConverter* const kFz_D3315_L[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasWaterLeakAlarm,
};

constexpr const char* kModels_D3315_L[] = { "3315-L" };

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"water_leak",  ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0500},  // ssIasZone
};

}  // namespace

extern const PreparedDefinition kDef_D3315_L{
    .zigbee_models=kModels_D3315_L, .zigbee_models_count=sizeof(kModels_D3315_L)/sizeof(kModels_D3315_L[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="3315-L", .vendor="Iris",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D3315_L, .from_zigbee_count=sizeof(kFz_D3315_L)/sizeof(kFz_D3315_L[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::iris
