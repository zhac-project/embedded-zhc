// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bitron AV2010/37 — hand-rewritten 2026-04-28.
// Water detector with siren.
// z2m-source: bitron.ts #AV2010/37.
//
// z2m bundle: fz.ias_water_leak_alarm_1
//   exposes [water_leak, battery_low].
//
// Mapped:
//   kFzIasWaterLeakAlarm — ssIasZone status → "water_leak" + "tamper" + "battery_low".
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bitron {
namespace {
const FzConverter* const kFz_AV2010_37[] = {
    &::zhc::generic::kFzIasWaterLeakAlarm,
};

constexpr const char* kModels_AV2010_37[] = { "AV2010/37", "902010/37" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"water_leak", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},
};

extern const PreparedDefinition kDef_AV2010_37{
    .zigbee_models=kModels_AV2010_37, .zigbee_models_count=sizeof(kModels_AV2010_37)/sizeof(kModels_AV2010_37[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AV2010/37", .vendor="Bitron",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AV2010_37, .from_zigbee_count=sizeof(kFz_AV2010_37)/sizeof(kFz_AV2010_37[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::bitron
