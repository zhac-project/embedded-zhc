// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Centralite 3315-Geu — hand-rewritten 2026-04-28.
// Water sensor.
// z2m-source: centralite.ts #3315-Geu.
//
// z2m bundle: fz.temperature + fz.ias_water_leak_alarm_1 + fz.battery
//   exposes [temperature, water_leak, battery_low, tamper, battery].
//
// Mapped:
//   kFzTemperature       — msTemperatureMeasurement attr 0x0000 → "temperature"
//   kFzIasWaterLeakAlarm — ssIasZone status bit 0 → "water_leak", bit 2 → "tamper",
//                          bit 3 → "battery_low"
//   kFzBattery           — genPowerCfg attrs 0x0020/0x0021 → "voltage" / "battery"
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::centralite {
namespace {
const FzConverter* const kFz_D3315_Geu[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasWaterLeakAlarm,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_D3315_Geu[] = { "3315-Geu" };

constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"water_leak",  ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0500},  // ssIasZone
};
}  // namespace

extern const PreparedDefinition kDef_D3315_Geu{
    .zigbee_models=kModels_D3315_Geu, .zigbee_models_count=sizeof(kModels_D3315_Geu)/sizeof(kModels_D3315_Geu[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="3315-Geu", .vendor="Centralite",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D3315_Geu, .from_zigbee_count=sizeof(kFz_D3315_Geu)/sizeof(kFz_D3315_Geu[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::centralite
