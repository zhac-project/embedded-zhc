// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Leedarson 7A-SS-ZABC-H0 — hand-rewritten 2026-04-29.
// 4-in-1 Sensor (motion + temperature + humidity + illuminance)
// z2m-source: leedarson.ts #7A-SS-ZABC-H0.
// Mapping: fromZigbee=[fz.battery, fz.ias_occupancy_alarm_1, fz.temperature,
//                       fz.humidity, fz.ignore_occupancy_report]
//          + extend=[m.illuminance()].
//          fz.ias_occupancy_alarm_1 -> kFzIasMotionAlarm (zoneStatus bit 0).
//          fz.ignore_occupancy_report is a noise filter — no expose / fz wired.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::leedarson {
namespace {
const FzConverter* const kFz_D7A_SS_ZABC_H0[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
};

constexpr const char* kModels_D7A_SS_ZABC_H0[] = { "ZB-SMART-PIRTH-V1" };

constexpr Expose kExposes_D7A_SS_ZABC_H0[] = {
    { "battery",     ExposeType::Numeric, Access::State, "%",    nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, Access::State, "mV",   nullptr, nullptr, 0 },
    { "occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0 },
    { "temperature", ExposeType::Numeric, Access::State, "°C",  nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, Access::State, "%",    nullptr, nullptr, 0 },
    { "illuminance", ExposeType::Numeric, Access::State, "lx",   nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_D7A_SS_ZABC_H0[] = {
    { 1, 0x0001 },  // genPowerCfg (battery)
    { 1, 0x0500 },  // ssIasZone (occupancy via IAS)
    { 1, 0x0402 },  // msTemperatureMeasurement
    { 1, 0x0405 },  // msRelativeHumidity
    { 1, 0x0400 },  // msIlluminanceMeasurement
};
}  // namespace

extern const PreparedDefinition kDef_D7A_SS_ZABC_H0{
    .zigbee_models=kModels_D7A_SS_ZABC_H0, .zigbee_models_count=sizeof(kModels_D7A_SS_ZABC_H0)/sizeof(kModels_D7A_SS_ZABC_H0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="7A-SS-ZABC-H0", .vendor="Leedarson",
    .meta=nullptr, .exposes=kExposes_D7A_SS_ZABC_H0, .exposes_count=sizeof(kExposes_D7A_SS_ZABC_H0)/sizeof(kExposes_D7A_SS_ZABC_H0[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D7A_SS_ZABC_H0, .from_zigbee_count=sizeof(kFz_D7A_SS_ZABC_H0)/sizeof(kFz_D7A_SS_ZABC_H0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D7A_SS_ZABC_H0, .bindings_count=sizeof(kBindings_D7A_SS_ZABC_H0)/sizeof(kBindings_D7A_SS_ZABC_H0[0]),
};

}  // namespace zhc::devices::leedarson
