// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: graduated to fix IAS dead-key + missing temperature.
// Zigbee smart water leak detector with external sensor.
//
// z2m-source: sinope.ts #WL4210 — fromZigbee
//   [fz.ias_water_leak_alarm_1, fz.temperature, fz.battery].
//
// See Sin_WL4200.cpp for rationale: typed kFzIasWaterLeakAlarm (emits
// water_leak) replaces the bare-"alarm" kFzIasZone; the on-board
// temperature sensor (kFzTemperature + 0x0402 bind) is restored.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sinope {
namespace {
const FzConverter* const kFz_WL4210[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasWaterLeakAlarm,
};

constexpr const char* kModels_WL4210[] = { "WL4210" };

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

extern const PreparedDefinition kDef_WL4210{
    .zigbee_models=kModels_WL4210, .zigbee_models_count=sizeof(kModels_WL4210)/sizeof(kModels_WL4210[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WL4210", .vendor="Sinope",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WL4210, .from_zigbee_count=sizeof(kFz_WL4210)/sizeof(kFz_WL4210[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::sinope
