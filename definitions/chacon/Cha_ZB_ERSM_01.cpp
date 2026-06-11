// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Chacon ZB-ERSM-01 — hand-rewritten 2026-06-11.
// Roller shutter module.
//
// z2m wires TWO modernExtend blocks:
//   m.windowCovering({controls:["lift"], coverInverted:true, coverMode:true})
//   m.commandsWindowCovering({commands:["open","close","stop"]})
//
// PARITY FIX: the auto-port kept only m.windowCovering's position half
// (kFzCoverPosition + position expose) and DROPPED m.commandsWindowCovering
// entirely. That modernExtend adds an `action` enum (open/close/stop)
// decoded from closuresWindowCovering cluster-specific commands
// 0x00 upOpen / 0x01 downClose / 0x02 stop — the module's own button
// presses. Wire the three generic command receivers + the action expose.
// Single channel (z2m declares no endpointNames) -> bare action, no
// endpoint_map.
//
// INFRA DEFER: m.windowCovering's coverMode:true adds a write-only
// `cover_mode` Composite (reversed/calibration/maintenance/led) routed via
// tz.cover_mode. There is no generic kTzCoverMode and ExposeType has no
// Composite variant — it carries no decodable device state, so it is left
// out (half-building config infra is out of scope for a parity pass).
// coverInverted:true is a presentation-side meta flag (z2m inverts the
// percentage for display); the position key itself is unchanged.
// z2m-source: chacon.ts #ZB-ERSM-01.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::chacon {
namespace {
const FzConverter* const kFz_ZB_ERSM_01[] = {
    &::zhc::generic::kFzCoverPosition,
    &::zhc::generic::kFzCommandCoverOpen,
    &::zhc::generic::kFzCommandCoverClose,
    &::zhc::generic::kFzCommandCoverStop,
};
const TzConverter* const kTz_ZB_ERSM_01[] = {
    &::zhc::generic::kTzCoverPosition,
};
constexpr const char* kModels_ZB_ERSM_01[] = { "ZB-ERSM-01" };

}  // namespace


// --- hand-edited exposes/bindings ---
constexpr Expose kAutoExposes[] = {
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0102},
};
// --- end hand-edited block ---

extern const PreparedDefinition kDef_ZB_ERSM_01{
    .zigbee_models=kModels_ZB_ERSM_01, .zigbee_models_count=sizeof(kModels_ZB_ERSM_01)/sizeof(kModels_ZB_ERSM_01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-ERSM-01", .vendor="Chacon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZB_ERSM_01, .from_zigbee_count=sizeof(kFz_ZB_ERSM_01)/sizeof(kFz_ZB_ERSM_01[0]),
    .to_zigbee=kTz_ZB_ERSM_01, .to_zigbee_count=sizeof(kTz_ZB_ERSM_01)/sizeof(kTz_ZB_ERSM_01[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::chacon
