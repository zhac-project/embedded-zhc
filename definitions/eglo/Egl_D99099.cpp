// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Eglo 99099 (ERCU_3groups_Zm) — 3-groups remote controller, not a switch.
//
// z2m models it as a pure command transmitter (no local relay):
//   fromZigbee: [fz.command_on, fz.awox_colors, fz.awox_refresh,
//                fz.awox_refreshColored, fz.command_off, fz.command_step,
//                fz.command_move, fz.command_move_to_level,
//                fz.command_move_to_color_temp, fz.command_stop,
//                fz.command_recall, fz.command_step_color_temperature]
//   exposes: e.action([...]) + e.numeric("action_group")
// The generated def wired kFzOnOff + an on/off TZ (both dead — kFzOnOff
// decodes attribute reports this remote never sends) and exposed a
// meaningless `state`. Re-wired to the generic command-action decoders so
// the button stream surfaces as `action` (on/off, brightness step/move/
// move_to_level/stop, scene recall_<n>, color-temp move/step). `action_group`
// is the ZCL group the press was addressed to (surfaced by the radio layer).
//
// DEFERRED (INFRA): the AwoX-specific raw frames — awox_colors
// (lightingColorCtrl `raw` colour bytes → red/blue/green/yellow),
// awox_refresh (genLevelCtrl `raw` → "refresh") and awox_refreshColored
// (commandMoveHue → "refresh_colored"). These decode vendor magic-byte
// `raw` payloads with no generic equivalent; mirroring the AwoX 33952
// precedent, the standard command stream (the primary button events) is
// wired and the manu-specific colour sub-actions are left for a dedicated
// awox raw-decoder if a user asks.
//
// z2m-source: eglo.ts #99099.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::eglo {
namespace {
const FzConverter* const kFz_D99099[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandStepColorTemp,
};
constexpr const char* kModels_D99099[] = { "ERCU_3groups_Zm" };

constexpr Expose kAutoExposes[] = {
    {"action",       ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"action_group", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006}, {1, 0x0008}, {1, 0x0005}, {1, 0x0300},
};
}  // namespace

extern const PreparedDefinition kDef_D99099{
    .zigbee_models=kModels_D99099, .zigbee_models_count=sizeof(kModels_D99099)/sizeof(kModels_D99099[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="99099", .vendor="Eglo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D99099, .from_zigbee_count=sizeof(kFz_D99099)/sizeof(kFz_D99099[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::eglo
