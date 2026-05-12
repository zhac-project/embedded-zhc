// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sunricher ZGRC-KEY-017 — auto-generated.
// ZGRC-KEY-017 (remote / scene emitter — no toZigbee).
// z2m-source: sunricher.ts #ZGRC-KEY-017.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sunricher {
namespace {
const FzConverter* const kFz_ZGRC_KEY_017[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_ZGRC_KEY_017[] = { "ZGRC-KEY-017" };

constexpr const char* kActions_ZGRC_KEY_017[] = {
    "on",
    "off",
    "brightness_step_up",
    "brightness_step_down",
    "brightness_move_up",
    "brightness_move_down",
    "brightness_stop",
    "color_temperature_move",
    "hue_move",
    "color_move",
    "recall_*",
};

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActions_ZGRC_KEY_017, sizeof(kActions_ZGRC_KEY_017)/sizeof(kActions_ZGRC_KEY_017[0])},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
    {1, 0x0001},
};

constexpr ::zhc::EndpointLabel kEndpoints_ZGRC_KEY_017[] = { {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6} };

}  // namespace

extern const PreparedDefinition kDef_ZGRC_KEY_017{
    .zigbee_models=kModels_ZGRC_KEY_017, .zigbee_models_count=sizeof(kModels_ZGRC_KEY_017)/sizeof(kModels_ZGRC_KEY_017[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZGRC-KEY-017", .vendor="Sunricher",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZGRC_KEY_017, .from_zigbee_count=sizeof(kFz_ZGRC_KEY_017)/sizeof(kFz_ZGRC_KEY_017[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_ZGRC_KEY_017,
    .endpoint_map_count = sizeof(kEndpoints_ZGRC_KEY_017)/sizeof(kEndpoints_ZGRC_KEY_017[0]),
};

}  // namespace zhc::devices::sunricher
