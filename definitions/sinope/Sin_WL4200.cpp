// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: graduated to fix IAS dead-key + missing temperature.
// Zigbee smart water leak detector.
//
// z2m-source: sinope.ts #WL4200 — fromZigbee
//   [fz.ias_water_leak_alarm_1, fz.temperature, fz.battery].
//
// The generated def lowered the generic kFzIasZone (emits the bare key
// "alarm") behind an expose that declared "alarm" — so the leak state
// never surfaced as the semantic `water_leak` the SPA/shadow expects.
// It also dropped the on-board temperature sensor entirely. Swap in the
// typed kFzIasWaterLeakAlarm (emits water_leak / tamper / battery_low)
// and add kFzTemperature + the msTemperatureMeasurement (0x0402) bind.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sinope {
namespace {
const FzConverter* const kFz_WL4200[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasWaterLeakAlarm,
};

constexpr const char* kModels_WL4200[] = { "WL4200" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"water_leak", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0500},  // ssIasZone
};

extern const PreparedDefinition kDef_WL4200{
    .zigbee_models=kModels_WL4200, .zigbee_models_count=sizeof(kModels_WL4200)/sizeof(kModels_WL4200[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WL4200", .vendor="Sinope",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WL4200, .from_zigbee_count=sizeof(kFz_WL4200)/sizeof(kFz_WL4200[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::sinope
