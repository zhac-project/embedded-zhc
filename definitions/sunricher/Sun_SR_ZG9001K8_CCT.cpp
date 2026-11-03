// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Sunricher SR-ZG9001K8-CCT CCT wall remote with 2 scene buttons
// (z2m v26.125.0 window). zigbeeModel ZGRC-KEY-008. m.battery + commandsOnOff +
// commandsLevelCtrl (step up/down) + commandsColorCtrl (colour-temperature step
// up/down) + commandsScenes (recall/store).
// z2m-source: sunricher.ts #SR-ZG9001K8-CCT.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sunricher {
namespace {
const FzConverter* const kFz_SR_ZG9001K8_CCT[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStepColorTemp,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandStore,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_SR_ZG9001K8_CCT[] = { "ZGRC-KEY-008" };

constexpr const char* kActions_SR_ZG9001K8_CCT[] = {
    "on",
    "off",
    "brightness_step_up",
    "brightness_step_down",
    "color_temperature_step_up",
    "color_temperature_step_down",
    "recall_*",
    "store_*",
};

constexpr Expose kExposes_SR_ZG9001K8_CCT[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActions_SR_ZG9001K8_CCT, sizeof(kActions_SR_ZG9001K8_CCT)/sizeof(kActions_SR_ZG9001K8_CCT[0])},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_SR_ZG9001K8_CCT[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
    {1, 0x0005},
    {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_SR_ZG9001K8_CCT{
    .zigbee_models=kModels_SR_ZG9001K8_CCT, .zigbee_models_count=sizeof(kModels_SR_ZG9001K8_CCT)/sizeof(kModels_SR_ZG9001K8_CCT[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SR-ZG9001K8-CCT", .vendor="Sunricher",
    .meta=nullptr, .exposes=kExposes_SR_ZG9001K8_CCT, .exposes_count=sizeof(kExposes_SR_ZG9001K8_CCT)/sizeof(kExposes_SR_ZG9001K8_CCT[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SR_ZG9001K8_CCT, .from_zigbee_count=sizeof(kFz_SR_ZG9001K8_CCT)/sizeof(kFz_SR_ZG9001K8_CCT[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SR_ZG9001K8_CCT, .bindings_count=sizeof(kBindings_SR_ZG9001K8_CCT)/sizeof(kBindings_SR_ZG9001K8_CCT[0]),
};

}  // namespace zhc::devices::sunricher
