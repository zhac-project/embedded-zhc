// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lutron LZL4BWHL01 — hand-curated (was wrong-bundled).
// Connected Bulb Remote — a battery remote. z2m wires it as an
// action-only device:
//   fromZigbee: [fz.command_step, fz.command_step,
//                fzLocal.custom_command_move_to_level, fz.command_stop]
//   toZigbee:   []   (cannot be controlled)
//   exposes:    action(["brightness_step_down", "brightness_step_up",
//                        "brightness_stop", "brightness_move_to_level"])
// i.e. the remote drives a paired bulb by emitting genLevelCtrl cluster
// commands (Step / MoveToLevel / Stop), surfaced as `action`.
//
// The auto-generator collapsed this into a single settable on/off light
// (kFzOnOff + kTzOnOff + a writable `state` expose) on EP1, with no action
// decode and a phantom relay. Replaced with the genLevelCtrl command
// decoder set + an `action` enum; the phantom state and toolbox dropped.
// Single endpoint (z2m has no deviceEndpoints) → no endpoint_map, bare
// `action`. z2m's custom_command_move_to_level only normalises a NaN level
// to 255 before delegating to fz.command_move_to_level — a JS-side edge case
// with no firmware bearing; the generic kFzCommandMoveToLevel decode matches.
//
// z2m-source: lutron.ts #LZL4BWHL01.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lutron {
namespace {
const FzConverter* const kFz_LZL4BWHL01[] = {
    &::zhc::generic::kFzCommandStep,         // genLevelCtrl Step → brightness_step_up/down
    &::zhc::generic::kFzCommandMoveToLevel,  // genLevelCtrl MoveToLevel → brightness_move_to_level
    &::zhc::generic::kFzCommandStop,         // genLevelCtrl Stop → brightness_stop
};
constexpr const char* kModels_LZL4BWHL01[] = { "LZL4BWHL01 Remote" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0008},   // genLevelCtrl (the remote's command source)
};

extern const PreparedDefinition kDef_LZL4BWHL01{
    .zigbee_models=kModels_LZL4BWHL01, .zigbee_models_count=sizeof(kModels_LZL4BWHL01)/sizeof(kModels_LZL4BWHL01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LZL4BWHL01", .vendor="Lutron",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_LZL4BWHL01, .from_zigbee_count=sizeof(kFz_LZL4BWHL01)/sizeof(kFz_LZL4BWHL01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::lutron
