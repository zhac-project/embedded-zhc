// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Hive MOT003 — hand-tuned.
// Motion sensor — z2m: fz.temperature + fz.ias_occupancy_alarm_1 + fz.battery.
// Endpoint 6 binds msTemperatureMeasurement + genPowerCfg.
// z2m-source: hive.ts #MOT003.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::hive {
namespace {
const FzConverter* const kFz_MOT003[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasZone,
};

constexpr const char* kModels_MOT003[] = { "MOT003" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {6, 0x0402},  // msTemperatureMeasurement
    {6, 0x0001},  // genPowerCfg
    {6, 0x0500},  // ssIasZone
};

extern const PreparedDefinition kDef_MOT003{
    .zigbee_models=kModels_MOT003, .zigbee_models_count=sizeof(kModels_MOT003)/sizeof(kModels_MOT003[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MOT003", .vendor="Hive",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MOT003, .from_zigbee_count=sizeof(kFz_MOT003)/sizeof(kFz_MOT003[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::hive
