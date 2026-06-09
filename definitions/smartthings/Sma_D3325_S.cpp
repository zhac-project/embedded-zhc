// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Smartthings 3325-S — IAS wrong-bit fix 2026-06-09.
// Motion sensor (2015 model).
// z2m-source: smartthings.ts #3325-S (fromZigbee: [fz.temperature,
//             fz.ias_occupancy_alarm_2, fz.battery]).
//
// z2m bundle: temperature + IAS motion (alarm_2 → occupancy, battery_low,
// tamper) + battery.
//
// FIX: z2m reads occupancy from `fz.ias_occupancy_alarm_2` (zoneStatus
// bit 1) ONLY. The previous port wired `kFzIasMotionAlarm` (bit 0), so
// occupancy never fired. Switched to `kFzIasMotionAlarm2` (bit 1).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smartthings {
namespace {
const FzConverter* const kFz_D3325_S[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasMotionAlarm2,
};

constexpr const char* kModels_D3325_S[] = { "3325-S" };

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0500},
};
}  // namespace

extern const PreparedDefinition kDef_D3325_S{
    .zigbee_models=kModels_D3325_S, .zigbee_models_count=sizeof(kModels_D3325_S)/sizeof(kModels_D3325_S[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="3325-S", .vendor="Smartthings",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D3325_S, .from_zigbee_count=sizeof(kFz_D3325_S)/sizeof(kFz_D3325_S[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::smartthings
