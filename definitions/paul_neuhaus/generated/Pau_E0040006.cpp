// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: PaulNeuhaus E0040006 — hand-rewritten remote.
// Q RGBW remote controller
// z2m-source: paul_neuhaus.ts #E0040006.
//
// z2m fromZigbee: command_step, command_enhanced_move_to_hue_and_saturation,
//   command_move_to_color_temp, command_on, command_off, command_color_loop_set,
//   command_move, command_stop, command_recall.
//
// PARTIAL: kFzCommandColorLoopSet and
// kFzCommandEnhancedMoveToHueAndSaturation absent in ZHC's generic
// bundle; those two action streams won't decode here. Substituted
// command_move_to_hue_and_saturation (closest equivalent).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::paul_neuhaus {
namespace {
const FzConverter* const kFz_E0040006[] = {
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandRecall,
};
constexpr const char* kModels_E0040006[] = { "JZ-RC-J4R" };

constexpr Expose kExposes_E0040006[] = {
    { "action",       ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0 },
    { "action_group", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_E0040006[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 }, { 1, 0x0005 },
};
}  // namespace

extern const PreparedDefinition kDef_E0040006{
    .zigbee_models=kModels_E0040006, .zigbee_models_count=sizeof(kModels_E0040006)/sizeof(kModels_E0040006[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E0040006", .vendor="PaulNeuhaus",
    .meta=nullptr, .exposes=kExposes_E0040006, .exposes_count=sizeof(kExposes_E0040006)/sizeof(kExposes_E0040006[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E0040006, .from_zigbee_count=sizeof(kFz_E0040006)/sizeof(kFz_E0040006[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_E0040006, .bindings_count=sizeof(kBindings_E0040006)/sizeof(kBindings_E0040006[0]),
};

}  // namespace zhc::devices::paul_neuhaus
