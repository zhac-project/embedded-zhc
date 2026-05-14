// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sunricher SR-ZG9001K8-DIM — auto-generated.
// SR-ZG9001K8-DIM (remote / scene emitter — no toZigbee).
// z2m-source: sunricher.ts #SR-ZG9001K8-DIM.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sunricher {
namespace {
const FzConverter* const kFz_SR_ZG9001K8_DIM[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_SR_ZG9001K8_DIM[] = { "ZG2833K8_EU05" };

constexpr const char* kActions_SR_ZG9001K8_DIM[] = {
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
};

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActions_SR_ZG9001K8_DIM, sizeof(kActions_SR_ZG9001K8_DIM)/sizeof(kActions_SR_ZG9001K8_DIM[0])},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {3, 0x0006},
    {4, 0x0006},
    {1, 0x0008},
    {2, 0x0008},
    {3, 0x0008},
    {4, 0x0008},
    {1, 0x0001},
};

constexpr ::zhc::EndpointLabel kEndpoints_SR_ZG9001K8_DIM[] = { {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4} };

}  // namespace


constexpr WhiteLabel kWhiteLabels_SR_ZG9001K8_DIM[] = {
    {"Sunricher","SR-ZG9001NK8-DIM"},
};
extern const PreparedDefinition kDef_SR_ZG9001K8_DIM{
    .zigbee_models=kModels_SR_ZG9001K8_DIM, .zigbee_models_count=sizeof(kModels_SR_ZG9001K8_DIM)/sizeof(kModels_SR_ZG9001K8_DIM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SR-ZG9001K8-DIM", .vendor="Sunricher",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_SR_ZG9001K8_DIM, .white_labels_count=sizeof(kWhiteLabels_SR_ZG9001K8_DIM)/sizeof(kWhiteLabels_SR_ZG9001K8_DIM[0]),
    .from_zigbee=kFz_SR_ZG9001K8_DIM, .from_zigbee_count=sizeof(kFz_SR_ZG9001K8_DIM)/sizeof(kFz_SR_ZG9001K8_DIM[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_SR_ZG9001K8_DIM,
    .endpoint_map_count = sizeof(kEndpoints_SR_ZG9001K8_DIM)/sizeof(kEndpoints_SR_ZG9001K8_DIM[0]),
};

}  // namespace zhc::devices::sunricher
