// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sunricher SR-ZG9001T4-DIM-EU — auto-generated.
// SR-ZG9001T4-DIM-EU (remote / scene emitter — no toZigbee).
// z2m-source: sunricher.ts #SR-ZG9001T4-DIM-EU.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sunricher {
namespace {
const FzConverter* const kFz_SR_ZG9001T4_DIM_EU[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandRecall,
};

constexpr const char* kModels_SR_ZG9001T4_DIM_EU[] = { "ZGRC-TEUR-005" };

constexpr const char* kActions_SR_ZG9001T4_DIM_EU[] = {
    "recall_*",
    "on",
    "off",
    "brightness_stop",
    "brightness_move_down",
    "brightness_move_up",
    "brightness_step_down",
    "brightness_step_up",
};

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActions_SR_ZG9001T4_DIM_EU, sizeof(kActions_SR_ZG9001T4_DIM_EU)/sizeof(kActions_SR_ZG9001T4_DIM_EU[0])},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};

}  // namespace

extern const PreparedDefinition kDef_SR_ZG9001T4_DIM_EU{
    .zigbee_models=kModels_SR_ZG9001T4_DIM_EU, .zigbee_models_count=sizeof(kModels_SR_ZG9001T4_DIM_EU)/sizeof(kModels_SR_ZG9001T4_DIM_EU[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SR-ZG9001T4-DIM-EU", .vendor="Sunricher",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SR_ZG9001T4_DIM_EU, .from_zigbee_count=sizeof(kFz_SR_ZG9001T4_DIM_EU)/sizeof(kFz_SR_ZG9001T4_DIM_EU[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::sunricher
