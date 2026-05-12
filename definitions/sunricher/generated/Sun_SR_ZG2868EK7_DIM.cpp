// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sunricher SR-ZG2868EK7-DIM — auto-generated.
// SR-ZG2868EK7-DIM (remote / scene emitter — no toZigbee).
// z2m-source: sunricher.ts #SR-ZG2868EK7-DIM.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sunricher {
namespace {
const FzConverter* const kFz_SR_ZG2868EK7_DIM[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_SR_ZG2868EK7_DIM[] = { "ZGRC-KEY-044" };

constexpr const char* kActions_SR_ZG2868EK7_DIM[] = {
    "on",
    "off",
    "brightness_step_up",
    "brightness_step_down",
    "brightness_move_up",
    "brightness_move_down",
    "brightness_stop",
    "recall_*",
    "store_*",
};

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActions_SR_ZG2868EK7_DIM, sizeof(kActions_SR_ZG2868EK7_DIM)/sizeof(kActions_SR_ZG2868EK7_DIM[0])},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_SR_ZG2868EK7_DIM{
    .zigbee_models=kModels_SR_ZG2868EK7_DIM, .zigbee_models_count=sizeof(kModels_SR_ZG2868EK7_DIM)/sizeof(kModels_SR_ZG2868EK7_DIM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SR-ZG2868EK7-DIM", .vendor="Sunricher",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SR_ZG2868EK7_DIM, .from_zigbee_count=sizeof(kFz_SR_ZG2868EK7_DIM)/sizeof(kFz_SR_ZG2868EK7_DIM[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::sunricher
