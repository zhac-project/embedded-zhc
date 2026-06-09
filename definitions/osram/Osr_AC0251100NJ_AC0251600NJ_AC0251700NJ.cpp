// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Osram AC0251100NJ/AC0251600NJ/AC0251700NJ — hand-rewritten remote.
// SMART+ Switch Mini (4-button ZLL scene/dimmer remote, "Lightify Switch
// Mini"/"... blue").
//
// Parity fix: this is a battery-powered REMOTE. z2m wires the genOnOff /
// genLevelCtrl / lightingColorCtrl *command* converters (fz.command_on/off,
// fz.command_move/stop/move_to_level, fz.command_move_to_color_temp,
// fz.command_move_hue, ...) and exposes a single `action` enum. The generated
// port instead lowered kFzOnOff (which decodes the genOnOff command into a
// `state` on/off — a phantom; a remote reports no state) and declared only a
// `state` expose, so every button press was dead. Re-pointed at the generic
// cluster-command action converters + an `action` enum, mirroring the
// iluminize 5144.01 / robb / legrand remote ports.
//
// Covers the z2m action set that maps to a wire command with a generic
// decoder: on, off, brightness_move_up/down, brightness_stop,
// brightness_move_to_level, color_temperature_move, hue_move/hue_stop.
// (z2m's move_to_hue / move_to_saturation use lightingColorCtrl cmd 0x00/0x03,
// which have no generic converter yet — left for a converter add, the same
// class of deferral as elsewhere in the tree.)
//
// z2m-source: osram.ts #AC0251100NJ/AC0251600NJ/AC0251700NJ.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::osram {
namespace {
const FzConverter* const kFz_AC0251100NJ_AC0251600NJ_AC0251700NJ[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStopWithOnOff,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveHue,
};

constexpr const char* kModels_AC0251100NJ_AC0251600NJ_AC0251700NJ[] = { "Lightify Switch Mini", "Lightify Switch Mini blue" };

constexpr Expose kExposes_AC0251100NJ_AC0251600NJ_AC0251700NJ[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_AC0251100NJ_AC0251600NJ_AC0251700NJ[] = {
    {1, 0x0001}, {1, 0x0006}, {1, 0x0008}, {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_AC0251100NJ_AC0251600NJ_AC0251700NJ{
    .zigbee_models=kModels_AC0251100NJ_AC0251600NJ_AC0251700NJ, .zigbee_models_count=sizeof(kModels_AC0251100NJ_AC0251600NJ_AC0251700NJ)/sizeof(kModels_AC0251100NJ_AC0251600NJ_AC0251700NJ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AC0251100NJ/AC0251600NJ/AC0251700NJ", .vendor="Osram",
    .meta=nullptr,
    .exposes=kExposes_AC0251100NJ_AC0251600NJ_AC0251700NJ,
    .exposes_count=sizeof(kExposes_AC0251100NJ_AC0251600NJ_AC0251700NJ)/sizeof(kExposes_AC0251100NJ_AC0251600NJ_AC0251700NJ[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AC0251100NJ_AC0251600NJ_AC0251700NJ, .from_zigbee_count=sizeof(kFz_AC0251100NJ_AC0251600NJ_AC0251700NJ)/sizeof(kFz_AC0251100NJ_AC0251600NJ_AC0251700NJ[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_AC0251100NJ_AC0251600NJ_AC0251700NJ,
    .bindings_count=sizeof(kBindings_AC0251100NJ_AC0251600NJ_AC0251700NJ)/sizeof(kBindings_AC0251100NJ_AC0251600NJ_AC0251700NJ[0]),
};

}  // namespace zhc::devices::osram
