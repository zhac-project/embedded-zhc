// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Iluminize 511.557 — hand-rewritten action-only wall dimmer.
// z2m: fromZigbee=[command_off, command_on, command_move_to_color_temp,
//                  command_move_to_color], toZigbee=[],
//      exposes=[action(["off","on","color_temperature_move","color_move"])].
// Mains-powered remote — no battery cluster, no controllable state.
// z2m-source: iluminize.ts #511.557.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::iluminize {
namespace {

const FzConverter* const kFz_D511_557[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
};

constexpr Expose kExposes_D511_557[] = {
    { "action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_D511_557[] = {
    { 1, 0x0006 }, { 1, 0x0300 },
};

constexpr const char* kModels_D511_557[] = { "ZG2801K2-G1-RGB-CCT-LEAD" };

}  // namespace

extern const PreparedDefinition kDef_D511_557{
    .zigbee_models=kModels_D511_557,
    .zigbee_models_count=sizeof(kModels_D511_557)/sizeof(kModels_D511_557[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="511.557", .vendor="Iluminize",
    .meta=nullptr,
    .exposes=kExposes_D511_557,
    .exposes_count=sizeof(kExposes_D511_557)/sizeof(kExposes_D511_557[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D511_557,
    .from_zigbee_count=sizeof(kFz_D511_557)/sizeof(kFz_D511_557[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D511_557,
    .bindings_count=sizeof(kBindings_D511_557)/sizeof(kBindings_D511_557[0]),
};

}  // namespace zhc::devices::iluminize
