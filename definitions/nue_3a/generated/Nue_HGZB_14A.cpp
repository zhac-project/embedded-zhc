// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nue3a HGZB-14A — hand-rewritten 2026-04-28.
// Water leakage sensor.
// z2m-source: nue_3a.ts #HGZB-14A.
//
// z2m bundle: fromZigbee=[fz.ias_water_leak_alarm_1, fz.battery];
//   exposes=[water_leak, battery_low, tamper, battery].
// We map fz.ias_water_leak_alarm_1 -> kFzIasWaterLeakAlarm so the IAS bit
// is decoded with the correct semantic key (water_leak), not the generic
// "alarm" emitted by kFzIasZone.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nue_3a {
namespace {
const FzConverter* const kFz_HGZB_14A[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasWaterLeakAlarm,
};

constexpr const char* kModels_HGZB_14A[] = { "FB56-WTS04HM1.1" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"water_leak",  ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone
};

extern const PreparedDefinition kDef_HGZB_14A{
    .zigbee_models=kModels_HGZB_14A, .zigbee_models_count=sizeof(kModels_HGZB_14A)/sizeof(kModels_HGZB_14A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HGZB-14A", .vendor="Nue3a",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HGZB_14A, .from_zigbee_count=sizeof(kFz_HGZB_14A)/sizeof(kFz_HGZB_14A[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nue_3a
