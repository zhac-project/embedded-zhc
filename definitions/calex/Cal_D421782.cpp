// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Calex 421782 — hand-rewritten command-style RGB wall controller.
// Smart Wall Switch, wall mounted RGB controller.
//
// Parity fix (was an on/off switch stub): the auto-generator emitted
// kFzOnOff + kTzOnOff (a controllable state path), but z2m models this as a
// REMOTE — `toZigbee:[]`, `fromZigbee:[command_off, command_on, command_step,
// command_move_to_color_temp, command_move, command_stop,
// command_enhanced_move_to_hue_and_saturation]`, `exposes:[e.action(...)]`.
// It emits cluster-specific commands (genOnOff / genLevelCtrl /
// lightingColorCtrl), not attribute reports, so the `state` expose was dead.
// Replaced with the seven generic command converters + an `action` expose.
// z2m-source: calex.ts #421782 (meta.disableActionGroup).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::calex {
namespace {
const FzConverter* const kFz_D421782[] = {
    &::zhc::generic::kFzCommandOff,                      // genOnOff   cmd 0x00 → "off"
    &::zhc::generic::kFzCommandOn,                       // genOnOff   cmd 0x01 → "on"
    &::zhc::generic::kFzCommandStep,                     // genLevelCtrl 0x02 → "brightness_step_*"
    &::zhc::generic::kFzCommandMoveToColorTemp,          // lightingColorCtrl 0x0A → "color_temperature_move"
    &::zhc::generic::kFzCommandMove,                     // genLevelCtrl 0x01 → "brightness_move_*"
    &::zhc::generic::kFzCommandStop,                     // genLevelCtrl 0x03 → "brightness_stop"
    &::zhc::generic::kFzCommandEnhancedMoveToHueAndSat,  // lightingColorCtrl 0x43 → "enhanced_move_to_hue_and_saturation"
};
constexpr const char* kModels_D421782[] = { "Smart Wall Switch " };  // trailing space is intentional (z2m)

constexpr Expose kExposes_D421782[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

// Remote binds the clusters it sources commands from.
constexpr BindingSpec kBindings_D421782[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl
};

}  // namespace


extern const PreparedDefinition kDef_D421782{
    .zigbee_models=kModels_D421782, .zigbee_models_count=sizeof(kModels_D421782)/sizeof(kModels_D421782[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="421782", .vendor="Calex",
    .meta=nullptr, .exposes=kExposes_D421782, .exposes_count=sizeof(kExposes_D421782)/sizeof(kExposes_D421782[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D421782, .from_zigbee_count=sizeof(kFz_D421782)/sizeof(kFz_D421782[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kBindings_D421782,.bindings_count=sizeof(kBindings_D421782)/sizeof(kBindings_D421782[0]),
};

}  // namespace zhc::devices::calex
