// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sunricher SR-ZG2836D5-G4 — auto-generated.
// SR-ZG2836D5-G4 (remote / scene emitter — no toZigbee).
// z2m-source: sunricher.ts #SR-ZG2836D5-G4.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sunricher {
namespace {
const FzConverter* const kFz_SR_ZG2836D5_G4[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_SR_ZG2836D5_G4[] = { "HK-ZRC-K5&RS-TL-G" };

constexpr const char* kActions_SR_ZG2836D5_G4[] = {
    "on_1",
    "off_1",
    "on_2",
    "off_2",
    "on_3",
    "off_3",
    "on_4",
    "off_4",
    "brightness_step_up",
    "brightness_step_down",
    "brightness_move_up",
    "brightness_move_down",
    "brightness_stop",
    "color_temperature_move",
    "hue_move",
    "color_move",
};

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActions_SR_ZG2836D5_G4, sizeof(kActions_SR_ZG2836D5_G4)/sizeof(kActions_SR_ZG2836D5_G4[0])},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {3, 0x0006},
    {4, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
    {1, 0x0001},
};

constexpr ::zhc::EndpointLabel kEndpoints_SR_ZG2836D5_G4[] = { {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4} };

}  // namespace

extern const PreparedDefinition kDef_SR_ZG2836D5_G4{
    .zigbee_models=kModels_SR_ZG2836D5_G4, .zigbee_models_count=sizeof(kModels_SR_ZG2836D5_G4)/sizeof(kModels_SR_ZG2836D5_G4[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SR-ZG2836D5-G4", .vendor="Sunricher",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SR_ZG2836D5_G4, .from_zigbee_count=sizeof(kFz_SR_ZG2836D5_G4)/sizeof(kFz_SR_ZG2836D5_G4[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_SR_ZG2836D5_G4,
    .endpoint_map_count = sizeof(kEndpoints_SR_ZG2836D5_G4)/sizeof(kEndpoints_SR_ZG2836D5_G4[0]),
};

}  // namespace zhc::devices::sunricher
