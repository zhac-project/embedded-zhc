// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sunricher ZG2819S-RGBW — auto-generated.
// ZG2819S-RGBW (remote / scene emitter — no toZigbee).
// z2m-source: sunricher.ts #ZG2819S-RGBW.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sunricher {
namespace {
const FzConverter* const kFz_ZG2819S_RGBW[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandStepColorTemp,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_ZG2819S_RGBW[] = { "ZG2819S-RGBW" };

constexpr const char* kActions_ZG2819S_RGBW[] = {
    "on",
    "off",
    "brightness_step_up",
    "brightness_step_down",
    "brightness_move_up",
    "brightness_move_down",
    "brightness_stop",
    "hue_move",
    "hue_stop",
    "color_move",
    "color_temperature_move",
    "recall_*",
    "store_*",
};

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActions_ZG2819S_RGBW, sizeof(kActions_ZG2819S_RGBW)/sizeof(kActions_ZG2819S_RGBW[0])},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
    {1, 0x0001},
};

constexpr ::zhc::EndpointLabel kEndpoints_ZG2819S_RGBW[] = { {"ep1", 1}, {"ep2", 2}, {"ep3", 3}, {"ep4", 4} };

}  // namespace

extern const PreparedDefinition kDef_ZG2819S_RGBW{
    .zigbee_models=kModels_ZG2819S_RGBW, .zigbee_models_count=sizeof(kModels_ZG2819S_RGBW)/sizeof(kModels_ZG2819S_RGBW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZG2819S-RGBW", .vendor="Sunricher",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZG2819S_RGBW, .from_zigbee_count=sizeof(kFz_ZG2819S_RGBW)/sizeof(kFz_ZG2819S_RGBW[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_ZG2819S_RGBW,
    .endpoint_map_count = sizeof(kEndpoints_ZG2819S_RGBW)/sizeof(kEndpoints_ZG2819S_RGBW[0]),
};

}  // namespace zhc::devices::sunricher
