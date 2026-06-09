// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Awox 33952 (ERCU_Zm) — battery remote controller, not a switch.
//
// z2m models it with a long action surface driven by the standard
// command converters plus three AwoX-specific raw decoders:
//   fromZigbee: [fz.command_on, fz.awox_colors, fz.awox_refresh,
//                fz.awox_refreshColored, fz.command_off, fz.command_step,
//                fz.command_move, fz.command_stop, fz.command_recall,
//                fz.command_step_color_temperature,
//                awox_color_ctrl, awox_level_ctrl, awox_scenes_raw]
//   exposes: e.action([...])
// It is a pure command transmitter (no local relay). The generated def
// wired kFzOnOff + an on/off TZ (both dead — kFzOnOff decodes attribute
// reports, which this remote never sends) and exposed a meaningless
// `state`. Re-wired to the generic command-action decoders so the button
// stream surfaces as `action` (on/off/toggle, brightness step/move/stop,
// scene recall_<n>, color-temp step).
//
// DEFERRED (INFRA): the AwoX-specific raw frames — awox_color_ctrl
// (lightingColorCtrl `raw` with colour byte 0xd6/0xd4/0xd2/0xd0 →
// color_blue/green/yellow/red + commandEnhancedMoveHue → light_movement),
// awox_level_ctrl (genLevelCtrl `raw` byte[1]==0xdf → "refresh") and
// awox_scenes_raw (genScenes `raw` last byte → recall_<n>). These decode
// vendor magic-byte `raw` payloads with no generic equivalent; mirroring
// the legrand precedent, the standard command stream (the primary button
// events) is wired and the manu-specific sub-actions are left for a
// dedicated awox raw-decoder if a user asks.
//
// z2m-source: awox.ts #33952.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::awox {
namespace {
const FzConverter* const kFz_D33952[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandStepColorTemp,
};
constexpr const char* kModels_D33952[] = { "ERCU_Zm", "TLSR82xx" };

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006}, {1, 0x0008}, {1, 0x0005}, {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_D33952{
    .zigbee_models=kModels_D33952, .zigbee_models_count=sizeof(kModels_D33952)/sizeof(kModels_D33952[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="33952", .vendor="Awox",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D33952, .from_zigbee_count=sizeof(kFz_D33952)/sizeof(kFz_D33952[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::awox
