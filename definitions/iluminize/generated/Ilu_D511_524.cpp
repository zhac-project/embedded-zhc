// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Iluminize 511.524 — hand-rewritten action-only wall dimmer (CCT, 4 zones).
// z2m: fromZigbee=[command_on, command_off, command_recall,
//                  command_move_to_color_temp, command_step, command_move,
//                  command_stop], toZigbee=[], meta.multiEndpoint=true,
//      exposes=[action(...)].
// z2m-source: iluminize.ts #511.524.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::iluminize {
namespace {

const FzConverter* const kFz_D511_524[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
};

constexpr Expose kExposes_D511_524[] = {
    { "action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_D511_524[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};

constexpr const char* kModels_D511_524[] = { "ZGRC-TEUR-003" };

}  // namespace

extern const PreparedDefinition kDef_D511_524{
    .zigbee_models=kModels_D511_524,
    .zigbee_models_count=sizeof(kModels_D511_524)/sizeof(kModels_D511_524[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="511.524", .vendor="Iluminize",
    .meta=nullptr,
    .exposes=kExposes_D511_524,
    .exposes_count=sizeof(kExposes_D511_524)/sizeof(kExposes_D511_524[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D511_524,
    .from_zigbee_count=sizeof(kFz_D511_524)/sizeof(kFz_D511_524[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D511_524,
    .bindings_count=sizeof(kBindings_D511_524)/sizeof(kBindings_D511_524[0]),
};

}  // namespace zhc::devices::iluminize
