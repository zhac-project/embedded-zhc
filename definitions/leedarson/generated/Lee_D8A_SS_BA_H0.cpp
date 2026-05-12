// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Leedarson 8A-SS-BA-H0 — hand-rewritten 2026-04-29.
// Motion Sensor
// z2m-source: leedarson.ts #8A-SS-BA-H0.
// Mapping: fromZigbee=[fz.battery, fz.ias_occupancy_alarm_1, fz.ignore_occupancy_report]
//          fz.ias_occupancy_alarm_1 -> kFzIasMotionAlarm (zoneStatus bit 0).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::leedarson {
namespace {
const FzConverter* const kFz_D8A_SS_BA_H0[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_D8A_SS_BA_H0[] = { "ZB-MotionSensor-S0000" };

constexpr Expose kExposes_D8A_SS_BA_H0[] = {
    { "battery",   ExposeType::Numeric, Access::State, "%",    nullptr, nullptr, 0 },
    { "voltage",   ExposeType::Numeric, Access::State, "mV",   nullptr, nullptr, 0 },
    { "occupancy", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_D8A_SS_BA_H0[] = {
    { 1, 0x0001 },  // genPowerCfg
    { 1, 0x0500 },  // ssIasZone
};
}  // namespace

extern const PreparedDefinition kDef_D8A_SS_BA_H0{
    .zigbee_models=kModels_D8A_SS_BA_H0, .zigbee_models_count=sizeof(kModels_D8A_SS_BA_H0)/sizeof(kModels_D8A_SS_BA_H0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="8A-SS-BA-H0", .vendor="Leedarson",
    .meta=nullptr, .exposes=kExposes_D8A_SS_BA_H0, .exposes_count=sizeof(kExposes_D8A_SS_BA_H0)/sizeof(kExposes_D8A_SS_BA_H0[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D8A_SS_BA_H0, .from_zigbee_count=sizeof(kFz_D8A_SS_BA_H0)/sizeof(kFz_D8A_SS_BA_H0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D8A_SS_BA_H0, .bindings_count=sizeof(kBindings_D8A_SS_BA_H0)/sizeof(kBindings_D8A_SS_BA_H0[0]),
};

}  // namespace zhc::devices::leedarson
