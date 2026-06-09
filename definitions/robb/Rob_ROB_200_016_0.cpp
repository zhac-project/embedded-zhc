// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Robb ROB_200-016-0 — hand-rewritten from a wrong on/off bundle.
// RGB CCT DIM 3-in-1 Zigbee remote.
// z2m-source: robb.ts #ROB_200-016-0 — fz.battery, fz.command_move_to_color,
//   fz.command_move_to_color_temp, fz.command_move_hue, fz.command_step,
//   fz.command_recall, fz.command_on/off/toggle, fz.command_stop, fz.command_move,
//   fz.command_color_loop_set, fz.command_enhanced_move_to_hue_and_saturation,
//   fz.command_move_to_hue_and_saturation.
//
// PARITY FIX (missing color actions): the original hand-port noted that the
// command-mode lightingColorCtrl decoders (move_to_color, move_hue,
// color_loop_set, enhanced/move_to_hue_and_saturation, move_to_color_temp) were
// "not yet present" in `_generic/_shared`. They EXIST now — wire all six so the
// remote's full RGB/CCT action set (color_move, color_temperature_move,
// hue_move, color_loop_set, enhanced_move_to_hue_and_saturation,
// move_to_hue_and_saturation) reaches consumers instead of only
// on/off/toggle/brightness/recall. All are lightingColorCtrl (0x0300) command-
// mode decoders disambiguated by command id (no clash with the genLevelCtrl
// Move at cmd 0x01 on cluster 0x0008).
// No to_zigbee path — this is a battery-powered remote/scene controller, the
// device is the genOnOff/genLevelCtrl/lightingColorCtrl client; we never write
// back.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::robb {
namespace {
const FzConverter* const kFz_ROB_200_016_0[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandMoveToColor,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveHue,
    &::zhc::generic::kFzCommandColorLoopSet,
    &::zhc::generic::kFzCommandEnhancedMoveToHueAndSat,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
};

constexpr const char* kModels_ROB_200_016_0[] = { "ROB_200-016-0" };
}  // namespace

constexpr Expose kAutoExposes[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0006},
    {1, 0x0008},
};

extern const PreparedDefinition kDef_ROB_200_016_0{
    .zigbee_models=kModels_ROB_200_016_0, .zigbee_models_count=sizeof(kModels_ROB_200_016_0)/sizeof(kModels_ROB_200_016_0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ROB_200-016-0", .vendor="Robb",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ROB_200_016_0, .from_zigbee_count=sizeof(kFz_ROB_200_016_0)/sizeof(kFz_ROB_200_016_0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::robb
