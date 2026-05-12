// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Mercator SMA02P — hand-rewritten (typed IAS motion).
// Motion detector.
//
// z2m: fz.ias_occupancy_alarm_1 + fz.battery + fz.ias_occupancy_alarm_1_report
//   exposes: occupancy, battery_low, tamper, battery, battery_voltage
//
// 2026-04-28p MERCATOR sweep — replaces generic kFzIasZone (which
// emits the generator-style `alarm` key) with kFzIasMotionAlarm so
// the surfaced key is the semantic z2m `occupancy` boolean.
//
// z2m-source: mercator.ts #SMA02P.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::mercator {
namespace {

const FzConverter* const kFz_SMA02P[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_SMA02P[] = { "TS0202" };
constexpr const char* kManus_SMA02P[] = { "_TYZB01_qjqgmqxr" };

constexpr Expose kExposes_SMA02P[] = {
    { "battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0 },
    { "occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0 },
    { "tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0 },
    { "battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_SMA02P[] = {
    { 1, 0x0001 },  // genPowerCfg
    { 1, 0x0500 },  // ssIasZone
};

}  // namespace

extern const PreparedDefinition kDef_SMA02P{
    .zigbee_models=kModels_SMA02P, .zigbee_models_count=sizeof(kModels_SMA02P)/sizeof(kModels_SMA02P[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SMA02P, .manufacturer_names_count=sizeof(kManus_SMA02P)/sizeof(kManus_SMA02P[0]),
    .model="SMA02P", .vendor="Mercator",
    .meta=nullptr,
    .exposes=kExposes_SMA02P, .exposes_count=sizeof(kExposes_SMA02P)/sizeof(kExposes_SMA02P[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SMA02P, .from_zigbee_count=sizeof(kFz_SMA02P)/sizeof(kFz_SMA02P[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SMA02P, .bindings_count=sizeof(kBindings_SMA02P)/sizeof(kBindings_SMA02P[0]),
};

}  // namespace zhc::devices::mercator
