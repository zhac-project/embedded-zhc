// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sunricher SR-ZG9001K12-DIM-Z5 — auto-generated.
// SR-ZG9001K12-DIM-Z5 (remote / scene emitter — no toZigbee).
// z2m-source: sunricher.ts #SR-ZG9001K12-DIM-Z5.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sunricher {
namespace {
const FzConverter* const kFz_SR_ZG9001K12_DIM_Z5[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_SR_ZG9001K12_DIM_Z5[] = { "ZGRC-KEY-012" };

constexpr const char* kActions_SR_ZG9001K12_DIM_Z5[] = {
    "on_1",
    "off_1",
    "brightness_move_up_1",
    "brightness_move_down_1",
    "brightness_stop_1",
    "on_2",
    "off_2",
    "brightness_move_up_2",
    "brightness_move_down_2",
    "brightness_stop_2",
    "on_3",
    "off_3",
    "brightness_move_up_3",
    "brightness_move_down_3",
    "brightness_stop_3",
    "on_4",
    "off_4",
    "brightness_move_up_4",
    "brightness_move_down_4",
    "brightness_stop_4",
    "on_5",
    "off_5",
    "brightness_move_up_5",
    "brightness_move_down_5",
    "brightness_stop_5",
};

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActions_SR_ZG9001K12_DIM_Z5, sizeof(kActions_SR_ZG9001K12_DIM_Z5)/sizeof(kActions_SR_ZG9001K12_DIM_Z5[0])},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {3, 0x0006},
    {4, 0x0006},
    {5, 0x0006},
    {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_SR_ZG9001K12_DIM_Z5{
    .zigbee_models=kModels_SR_ZG9001K12_DIM_Z5, .zigbee_models_count=sizeof(kModels_SR_ZG9001K12_DIM_Z5)/sizeof(kModels_SR_ZG9001K12_DIM_Z5[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SR-ZG9001K12-DIM-Z5", .vendor="Sunricher",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SR_ZG9001K12_DIM_Z5, .from_zigbee_count=sizeof(kFz_SR_ZG9001K12_DIM_Z5)/sizeof(kFz_SR_ZG9001K12_DIM_Z5[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::sunricher
