// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Paulmann 500.67 — hand-curated (was wrong-bundled).
// RGB remote control — battery-powered (single endpoint), NOT a settable
// on/off light. The auto-generator emitted kFzOnOff + kTzOnOff + a writable
// `state`, so every press was dead. The remote actually drives a bound
// light by emitting genOnOff / genLevelCtrl / lightingColorCtrl cluster
// commands, which the action decoders surface on the `action` channel.
//
// z2m fromZigbee: command_on, command_off, command_toggle, command_step,
//   command_move_to_color_temp, command_move_to_color, command_stop,
//   command_move, command_color_loop_set,
//   command_enhanced_move_to_hue_and_saturation, tint_scene
// z2m toZigbee: [] (cannot be controlled).
// z2m exposes: e.action([on, off, toggle, brightness_step_up,
//   brightness_step_down, color_temperature_move, color_move,
//   brightness_stop, brightness_move_down, brightness_move_up,
//   color_loop_set, enhanced_move_to_hue_and_saturation, scene_*]).
//
// Coverage caveat: `fz.tint_scene` is a Tint-specific genBasic *write*
// recall (the scene_* actions) and has no generic ZHC equivalent today —
// same situation as innr's rc_110_level_to_scene. All other presses are
// fully decoded. Single endpoint, so `action` is emitted bare (no suffix).
//
// z2m-source: paulmann.ts #500.67.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::paulmann {
namespace {
const FzConverter* const kFz_D500_67[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveToColor,
    &::zhc::generic::kFzCommandColorLoopSet,
    &::zhc::generic::kFzCommandEnhancedMoveToHueAndSat,
};
constexpr const char* kModels_D500_67[] = { "RemoteControl", "50067" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    // `action` value carries on/off/toggle/brightness_step_*/brightness_move_*/
    // brightness_stop/color_temperature_move/color_move/color_loop_set/
    // enhanced_move_to_hue_and_saturation (z2m action set, minus scene_*).
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0300},   // lightingColorCtrl
};

extern const PreparedDefinition kDef_D500_67{
    .zigbee_models=kModels_D500_67, .zigbee_models_count=sizeof(kModels_D500_67)/sizeof(kModels_D500_67[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="500.67", .vendor="Paulmann",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D500_67, .from_zigbee_count=sizeof(kFz_D500_67)/sizeof(kFz_D500_67[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::paulmann
