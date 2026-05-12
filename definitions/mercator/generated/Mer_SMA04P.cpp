// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Mercator SMA04P — hand-rewritten (typed IAS contact).
// Contact sensor.
//
// z2m: fz.ias_contact_alarm_1 + fz.battery + fz.ias_contact_alarm_1_report
//   exposes: contact, battery_low, tamper, battery, battery_voltage
//
// 2026-04-28p MERCATOR sweep — replaces generic kFzIasZone (which
// emits the generator-style `alarm` key) with kFzIasContactAlarm so
// the surfaced key is the semantic z2m `contact` boolean.
//
// z2m-source: mercator.ts #SMA04P.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::mercator {
namespace {

const FzConverter* const kFz_SMA04P[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,
};

constexpr const char* kModels_SMA04P[] = { "TS0203" };
constexpr const char* kManus_SMA04P[] = { "_TZ3000_wbrlnkm9" };

constexpr Expose kExposes_SMA04P[] = {
    { "battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0 },
    { "contact",     ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0 },
    { "tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0 },
    { "battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_SMA04P[] = {
    { 1, 0x0001 },  // genPowerCfg
    { 1, 0x0500 },  // ssIasZone
};

}  // namespace

extern const PreparedDefinition kDef_SMA04P{
    .zigbee_models=kModels_SMA04P, .zigbee_models_count=sizeof(kModels_SMA04P)/sizeof(kModels_SMA04P[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SMA04P, .manufacturer_names_count=sizeof(kManus_SMA04P)/sizeof(kManus_SMA04P[0]),
    .model="SMA04P", .vendor="Mercator",
    .meta=nullptr,
    .exposes=kExposes_SMA04P, .exposes_count=sizeof(kExposes_SMA04P)/sizeof(kExposes_SMA04P[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SMA04P, .from_zigbee_count=sizeof(kFz_SMA04P)/sizeof(kFz_SMA04P[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SMA04P, .bindings_count=sizeof(kBindings_SMA04P)/sizeof(kBindings_SMA04P[0]),
};

}  // namespace zhc::devices::mercator
