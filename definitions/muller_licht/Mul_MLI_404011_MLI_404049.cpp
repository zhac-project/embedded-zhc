// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: MullerLicht MLI-404011/MLI-404049 — hand-rewritten from a wrong on/off bundle.
// Tint remote control (4-button RGB+CCT remote).
// z2m-source: muller_licht.ts #MLI-404011/MLI-404049.
//
// z2m fromZigbee:
//   command_on, command_off, command_toggle, command_step,
//   tint404011_move_to_color_temp, command_move_to_color, tint_scene,
//   command_stop, command_move
//
// genOnOff/genLevelCtrl covers on/off/toggle/step/move/stop. The colour
// wheel is now wired too:
//   * color_temperature_move ← kFzCommandMoveToColorTemp (lightingColorCtrl
//     cmd 0x0A). z2m's `tint404011_move_to_color_temp` adds a synthetic
//     _up/_down suffix derived from device-internal state; we emit the bare
//     `color_temperature_move` string the action enum declares (the suffix
//     is z2m cosmetic state-tracking, not on the wire).
//   * color_move              ← kFzCommandMoveToColor (cmd 0x07).
// Previously both colour-wheel action groups were dead enum entries.
//
// scene_1..6 (z2m `fz.tint_scene`, a genBasic *Write Attributes* of attr
// 0x4005) still degrade silently: the foundation Write-attribute parser
// does not populate `msg.payload`, so the generic kFzTintScene converter
// can never fire. Wiring it would be inert — this is an INFRA gap tracked
// for a shared-decoder fix, not a per-device port. See
// docs/MULLER_LICHT_PARITY.md.
//
// Action enum carries the strings z2m exposes; consumers see a pure
// `action=<event>` string. Battery-only remote — no to_zigbee, the device
// is the genOnOff / genLevelCtrl client and we never write back.
//
// Fingerprint covers both the stock Müller-Licht model strings AND the
// Tuya white-label TS1001 with manufacturer "_TZ3000_bdbb0fon".
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::muller_licht {
namespace {
const FzConverter* const kFz_MLI_404011_MLI_404049[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandMoveToColorTemp,  // color_temperature_move
    &::zhc::generic::kFzCommandMoveToColor,      // color_move
};

constexpr const char* kModels_MLI_404011_MLI_404049[] = { "ZBT-Remote-ALL-RGBW", "TS1001" };
constexpr const char* kManus_MLI_404011_MLI_404049[]  = { "_TZ3000_bdbb0fon" };
constexpr const char* kActionValues_MLI_404011_MLI_404049[] = {
    "on", "off", "toggle",
    "brightness_step_up", "brightness_step_down",
    "brightness_move_up", "brightness_move_down", "brightness_stop",
    "color_temperature_move", "color_move",
    "scene_1", "scene_2", "scene_3", "scene_4", "scene_5", "scene_6",
};
}  // namespace

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr,
     kActionValues_MLI_404011_MLI_404049,
     sizeof(kActionValues_MLI_404011_MLI_404049)/sizeof(kActionValues_MLI_404011_MLI_404049[0])},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

extern const PreparedDefinition kDef_MLI_404011_MLI_404049{
    .zigbee_models=kModels_MLI_404011_MLI_404049, .zigbee_models_count=sizeof(kModels_MLI_404011_MLI_404049)/sizeof(kModels_MLI_404011_MLI_404049[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_MLI_404011_MLI_404049, .manufacturer_names_count=sizeof(kManus_MLI_404011_MLI_404049)/sizeof(kManus_MLI_404011_MLI_404049[0]),
    .model="MLI-404011/MLI-404049", .vendor="MullerLicht",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MLI_404011_MLI_404049, .from_zigbee_count=sizeof(kFz_MLI_404011_MLI_404049)/sizeof(kFz_MLI_404011_MLI_404049[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::muller_licht
