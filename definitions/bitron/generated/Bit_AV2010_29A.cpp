// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bitron AV2010/29A — hand-rewritten 2026-04-28.
// Outdoor siren (with squawk).
// z2m-source: bitron.ts #AV2010/29A.
//
// z2m bundle: fz.ias_siren + tz.warning + tz.squawk
//   exposes [warning, squawk, battery_low, tamper].
//
// Mapped:
//   kFzIasZone — ssIasZone raw status → "alarm_1" + "alarm_2" + "tamper"
//                + "battery_low". `fz.ias_siren` is a multi-bit composite
//                that doesn't fit a single semantic-label kFzIas*Alarm.
//                Generic decoder keeps both alarm bits, which is the closest
//                approximation we can offer.
//   kTzWarning — ssIasWd warning command.
//   NOTE: tz.squawk has no generic ZHC analogue yet; the squawk write path
//         is unwired (see docs/BITRON_PARITY.md).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bitron {
namespace {
const FzConverter* const kFz_AV2010_29A[] = {
    &::zhc::generic::kFzIasZone,
};
const TzConverter* const kTz_AV2010_29A[] = {
    &::zhc::generic::kTzWarning,
};
constexpr const char* kModels_AV2010_29A[] = { "AV2010/29A", "902010/29A" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"alarm_1", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"alarm_2", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},
    {1, 0x0502},
};

extern const PreparedDefinition kDef_AV2010_29A{
    .zigbee_models=kModels_AV2010_29A, .zigbee_models_count=sizeof(kModels_AV2010_29A)/sizeof(kModels_AV2010_29A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AV2010/29A", .vendor="Bitron",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AV2010_29A, .from_zigbee_count=sizeof(kFz_AV2010_29A)/sizeof(kFz_AV2010_29A[0]),
    .to_zigbee=kTz_AV2010_29A, .to_zigbee_count=sizeof(kTz_AV2010_29A)/sizeof(kTz_AV2010_29A[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::bitron
