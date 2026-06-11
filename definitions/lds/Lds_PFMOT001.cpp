// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lds PFMOT001 — hand-authored (was MISSING entirely).
// Hive Pet Friendly Motion sensor. z2m extend = m.battery() +
// m.iasZoneAlarm({zoneType:"occupancy", zoneAttributes:["alarm_1","tamper",
// "battery_low"]}) → battery + IAS motion. zoneType "occupancy" with alarm_1
// (zoneStatus bit 0) is z2m's fz.ias_occupancy_alarm_1 → semantic key
// `occupancy`; tamper (bit 2) + battery_low (bit 3) co-decode. The auto-port
// dropped this definition; this file adds it: kFzBattery (genPowerCfg 0x0001)
// + kFzIasMotionAlarm (ssIasZone 0x0500, bit 0 → occupancy).
// z2m-source: lds.ts #PFMOT001.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lds {
namespace {
const FzConverter* const kFz_PFMOT001[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
};
constexpr const char* kModels_PFMOT001[] = { "PFMOT001" };

constexpr Expose kExposes_PFMOT001[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_PFMOT001[] = {
    {1, 0x0001},  // genPowerCfg (battery)
    {1, 0x0500},  // ssIasZone
};

}  // namespace

extern const PreparedDefinition kDef_PFMOT001{
    .zigbee_models=kModels_PFMOT001, .zigbee_models_count=sizeof(kModels_PFMOT001)/sizeof(kModels_PFMOT001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PFMOT001", .vendor="Lds",
    .meta=nullptr, .exposes=kExposes_PFMOT001, .exposes_count=sizeof(kExposes_PFMOT001)/sizeof(kExposes_PFMOT001[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_PFMOT001, .from_zigbee_count=sizeof(kFz_PFMOT001)/sizeof(kFz_PFMOT001[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_PFMOT001, .bindings_count=sizeof(kBindings_PFMOT001)/sizeof(kBindings_PFMOT001[0]),
};

}  // namespace zhc::devices::lds
