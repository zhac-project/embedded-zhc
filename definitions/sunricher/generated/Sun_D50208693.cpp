// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sunricher 50208693 — auto-generated.
// 50208693 (remote / scene emitter — no toZigbee).
// z2m-source: sunricher.ts #50208693.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sunricher {
namespace {
const FzConverter* const kFz_D50208693[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandStep,
};

constexpr const char* kModels_D50208693[] = { "ZGRC-KEY-009" };

constexpr const char* kActions_D50208693[] = {
    "on",
    "off",
    "brightness_move_up",
    "brightness_move_down",
    "brightness_move_stop",
    "brightness_step_up",
    "brightness_step_down",
    "recall_1",
    "recall_2",
};

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActions_D50208693, sizeof(kActions_D50208693)/sizeof(kActions_D50208693[0])},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_D50208693{
    .zigbee_models=kModels_D50208693, .zigbee_models_count=sizeof(kModels_D50208693)/sizeof(kModels_D50208693[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="50208693", .vendor="Sunricher",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D50208693, .from_zigbee_count=sizeof(kFz_D50208693)/sizeof(kFz_D50208693[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::sunricher
