// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bitron AV2010/24A — hand-rewritten 2026-04-28.
// Optical smoke detector (hardware version v2).
// z2m-source: bitron.ts #AV2010/24A.
//
// z2m bundle: fz.ias_smoke_alarm_1 + tz.warning
//   exposes [smoke, battery_low, tamper, warning].
//
// Mapped:
//   kFzIasSmokeAlarm — ssIasZone status → "smoke" + "tamper" + "battery_low".
//   kTzWarning       — ssIasWd warning command.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bitron {
namespace {
const FzConverter* const kFz_AV2010_24A[] = {
    &::zhc::generic::kFzIasSmokeAlarm,
};
const TzConverter* const kTz_AV2010_24A[] = {
    &::zhc::generic::kTzWarning,
};
constexpr const char* kModels_AV2010_24A[] = { "AV2010/24A", "902010/24A" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"smoke", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},
};

extern const PreparedDefinition kDef_AV2010_24A{
    .zigbee_models=kModels_AV2010_24A, .zigbee_models_count=sizeof(kModels_AV2010_24A)/sizeof(kModels_AV2010_24A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AV2010/24A", .vendor="Bitron",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AV2010_24A, .from_zigbee_count=sizeof(kFz_AV2010_24A)/sizeof(kFz_AV2010_24A[0]),
    .to_zigbee=kTz_AV2010_24A, .to_zigbee_count=sizeof(kTz_AV2010_24A)/sizeof(kTz_AV2010_24A[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::bitron
