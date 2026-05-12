// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Iluminize 511.544 — hand-rewritten action-only wall dimmer.
// Zigbee 3.0 wall dimmer RGBW 4 zones.
// z2m: fromZigbee=[command_move_to_color, command_move_hue, command_on,
//                  command_off, command_move], toZigbee=[],
//      exposes=[action(...)]. No battery cluster.
// z2m-source: iluminize.ts #511.544.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::iluminize {
namespace {

const FzConverter* const kFz_D511_544[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
    &::zhc::generic::kFzCommandMove,
};

constexpr Expose kExposes_D511_544[] = {
    { "action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_D511_544[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};

constexpr const char* kModels_D511_544[] = { "ZGRC-TEUR-001" };

}  // namespace

extern const PreparedDefinition kDef_D511_544{
    .zigbee_models=kModels_D511_544,
    .zigbee_models_count=sizeof(kModels_D511_544)/sizeof(kModels_D511_544[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="511.544", .vendor="Iluminize",
    .meta=nullptr,
    .exposes=kExposes_D511_544,
    .exposes_count=sizeof(kExposes_D511_544)/sizeof(kExposes_D511_544[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D511_544,
    .from_zigbee_count=sizeof(kFz_D511_544)/sizeof(kFz_D511_544[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D511_544,
    .bindings_count=sizeof(kBindings_D511_544)/sizeof(kBindings_D511_544[0]),
};

}  // namespace zhc::devices::iluminize
