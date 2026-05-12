// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lifecontrol MCLH-05 — hand-rewritten (vendor parity sweep).
// Motion sensor.
// z2m-source: lifecontrol.ts #MCLH-05.
//
// Wire-level: ssIasZone status-change → "occupancy" + "tamper"
// + "battery_low" via the typed IAS converter (z2m m.iasZoneAlarm
// with zoneType:"occupancy"); battery via genPowerCfg.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lifecontrol {
namespace {
const FzConverter* const kFz_MCLH_05[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_MCLH_05[] = { "Motion_Sensor" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone
};

extern const PreparedDefinition kDef_MCLH_05{
    .zigbee_models=kModels_MCLH_05, .zigbee_models_count=sizeof(kModels_MCLH_05)/sizeof(kModels_MCLH_05[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MCLH-05", .vendor="Lifecontrol",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MCLH_05, .from_zigbee_count=sizeof(kFz_MCLH_05)/sizeof(kFz_MCLH_05[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::lifecontrol
