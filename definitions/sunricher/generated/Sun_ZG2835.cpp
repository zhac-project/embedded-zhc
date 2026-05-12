// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sunricher ZG2835 — auto-generated.
// ZG2835 (remote / scene emitter — no toZigbee).
// z2m-source: sunricher.ts #ZG2835.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sunricher {
namespace {
const FzConverter* const kFz_ZG2835[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMoveToLevel,
};

constexpr const char* kModels_ZG2835[] = { "ZG2835" };

constexpr const char* kActions_ZG2835[] = {
    "on",
    "off",
    "brightness_move_to_level",
};

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActions_ZG2835, sizeof(kActions_ZG2835)/sizeof(kActions_ZG2835[0])},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};

}  // namespace

extern const PreparedDefinition kDef_ZG2835{
    .zigbee_models=kModels_ZG2835, .zigbee_models_count=sizeof(kModels_ZG2835)/sizeof(kModels_ZG2835[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZG2835", .vendor="Sunricher",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZG2835, .from_zigbee_count=sizeof(kFz_ZG2835)/sizeof(kFz_ZG2835[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::sunricher
