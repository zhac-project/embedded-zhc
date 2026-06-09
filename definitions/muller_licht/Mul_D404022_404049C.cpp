// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: MullerLicht 404022/404049C — hand-rewritten from a wrong on/off bundle.
// Tint dim remote control (10-scene RGB+CCT remote).
// z2m-source: muller_licht.ts #404022/404049C.
//
// z2m fromZigbee:
//   command_on, command_off, command_step, command_move, command_stop,
//   command_move_to_color_temp, command_move_to_color, tint_scene
//
// genOnOff/genLevelCtrl covers on/off/step/move/stop and
// kFzCommandMoveToColorTemp covers color_temperature_move. The colour
// wheel is now wired too:
//   * color_move ← kFzCommandMoveToColor (lightingColorCtrl cmd 0x07).
// Previously color_move was a dead enum entry with no decoder.
//
// scene_1..10 (z2m `fz.tint_scene`, a genBasic *Write Attributes* of attr
// 0x4005) still degrade silently: the foundation Write-attribute parser
// does not populate `msg.payload`, so the generic kFzTintScene converter
// can never fire. Wiring it would be inert — INFRA gap, see
// docs/MULLER_LICHT_PARITY.md.
//
// Both `zigbeeModel: ["tint-Remote-white"]` and the Tuya-rebrand
// fingerprint `{modelID: "Remote Control", manufacturerName: "MLI"}`
// land here.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::muller_licht {
namespace {
const FzConverter* const kFz_D404022_404049C[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandMoveToColorTemp,  // color_temperature_move
    &::zhc::generic::kFzCommandMoveToColor,      // color_move
};

constexpr const char* kModels_D404022_404049C[] = { "tint-Remote-white", "Remote Control" };
constexpr const char* kManus_D404022_404049C[]  = { "MLI" };
constexpr const char* kActionValues_D404022_404049C[] = {
    "on", "off",
    "brightness_step_up", "brightness_step_down",
    "brightness_move_up", "brightness_move_down", "brightness_stop",
    "color_temperature_move", "color_move",
    "scene_1", "scene_2", "scene_3", "scene_4", "scene_5",
    "scene_6", "scene_7", "scene_8", "scene_9", "scene_10",
};
}  // namespace

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr,
     kActionValues_D404022_404049C,
     sizeof(kActionValues_D404022_404049C)/sizeof(kActionValues_D404022_404049C[0])},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};


constexpr WhiteLabel kWhiteLabels_D404022_404049C[] = {
    {"Müller Licht","404049D"},
};
extern const PreparedDefinition kDef_D404022_404049C{
    .zigbee_models=kModels_D404022_404049C, .zigbee_models_count=sizeof(kModels_D404022_404049C)/sizeof(kModels_D404022_404049C[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_D404022_404049C, .manufacturer_names_count=sizeof(kManus_D404022_404049C)/sizeof(kManus_D404022_404049C[0]),
    .model="404022/404049C", .vendor="MullerLicht",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_D404022_404049C, .white_labels_count=sizeof(kWhiteLabels_D404022_404049C)/sizeof(kWhiteLabels_D404022_404049C[0]),
    .from_zigbee=kFz_D404022_404049C, .from_zigbee_count=sizeof(kFz_D404022_404049C)/sizeof(kFz_D404022_404049C[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::muller_licht
