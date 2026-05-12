// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Iris 3326-L — hand-rewritten 2026-04-28q.
// Motion and temperature sensor.
// z2m-source: iris.ts #3326-L.
//   fromZigbee: [fz.ias_occupancy_alarm_2, fz.temperature, fz.battery]
//   exposes:    [occupancy, battery_low, temperature, battery]
//   Note: z2m's `ias_occupancy_alarm_2` reads bit 1 (alarm_2); the generic
//   `kFzIasMotionAlarm` decoder reads bit 0 (alarm_1). Output key is the same;
//   bit-selection mismatch is a known cross-vendor gap (centralite 3328-G also
//   notes this). Pending `kFzIasMotionAlarm2` landing.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::iris {
namespace {
const FzConverter* const kFz_D3326_L[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasMotionAlarm2,
};

constexpr const char* kModels_D3326_L[] = { "3326-L" };

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0500},  // ssIasZone
};

}  // namespace

extern const PreparedDefinition kDef_D3326_L{
    .zigbee_models=kModels_D3326_L, .zigbee_models_count=sizeof(kModels_D3326_L)/sizeof(kModels_D3326_L[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="3326-L", .vendor="Iris",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D3326_L, .from_zigbee_count=sizeof(kFz_D3326_L)/sizeof(kFz_D3326_L[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::iris
