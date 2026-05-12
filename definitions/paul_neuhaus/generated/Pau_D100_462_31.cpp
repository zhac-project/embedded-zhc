// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: PaulNeuhaus 100.462.31 — hand-rewritten remote.
// Q-REMOTE
// z2m-source: paul_neuhaus.ts #100.462.31.
//
// z2m fromZigbee: command_on, command_off, command_toggle, command_step,
//   command_move_to_color_temp, command_stop, command_move_to_color,
//   command_move, command_color_loop_set,
//   command_enhanced_move_to_hue_and_saturation, tint_scene,
//   command_recall.
// z2m exposes: action(...), action_group()
// z2m toZigbee: []  (receive-only remote)
//
// PARTIAL: ZHC's _generic shared bundle currently lacks
// kFzCommandColorLoopSet, kFzCommandEnhancedMoveToHueAndSaturation, and
// kFzTintScene — those four action variants will not surface on the
// state stream. Wired here:
//   command_on/off/toggle/step/move/stop/recall +
//   command_move_to_color_temp + command_move_to_color +
//   command_move_color_temperature.
// Bindings 0x0005 (genScenes) added for command_recall scene group.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::paul_neuhaus {
namespace {
const FzConverter* const kFz_D100_462_31[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandMoveToColor,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandMoveColorTemperature,
    &::zhc::generic::kFzCommandRecall,
};
// receive-only remote — no toZigbee[] array.
constexpr const char* kModels_D100_462_31[] = { "NLG-remote", "Neuhaus remote" };

constexpr Expose kExposes_D100_462_31[] = {
    { "action",       ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0 },
    { "action_group", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_D100_462_31[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 }, { 1, 0x0005 },
};
}  // namespace

extern const PreparedDefinition kDef_D100_462_31{
    .zigbee_models=kModels_D100_462_31, .zigbee_models_count=sizeof(kModels_D100_462_31)/sizeof(kModels_D100_462_31[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="100.462.31", .vendor="PaulNeuhaus",
    .meta=nullptr, .exposes=kExposes_D100_462_31, .exposes_count=sizeof(kExposes_D100_462_31)/sizeof(kExposes_D100_462_31[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D100_462_31, .from_zigbee_count=sizeof(kFz_D100_462_31)/sizeof(kFz_D100_462_31[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D100_462_31, .bindings_count=sizeof(kBindings_D100_462_31)/sizeof(kBindings_D100_462_31[0]),
};

}  // namespace zhc::devices::paul_neuhaus
