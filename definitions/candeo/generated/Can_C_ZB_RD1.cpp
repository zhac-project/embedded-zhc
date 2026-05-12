// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Candeo C-ZB-RD1.
// Zigbee rotary dimmer
// z2m-source: candeo.ts #C-ZB-RD1.
#include "definitions/_generic/_shared.hpp"


namespace zhc::devices::candeo {
namespace {
const FzConverter* const kFz_C_ZB_RD1[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzPowerOnBehavior1
};
const TzConverter* const kTz_C_ZB_RD1[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzPowerOnBehavior1
};
constexpr const char* kModels_C_ZB_RD1[] = { "C-ZB-RD1" };

constexpr Expose kAutoExposes[] = {
    {"state",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy",            ExposeType::Numeric, Access::State,    "kWh",   nullptr, nullptr, 0},
    {"power",             ExposeType::Numeric, Access::State,    "W",     nullptr, nullptr, 0},
    {"voltage",           ExposeType::Numeric, Access::State,    "V",     nullptr, nullptr, 0},
    {"current",           ExposeType::Numeric, Access::State,    "A",     nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0}
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0702},
    {1, 0x0B04}
};

}  // namespace

extern const PreparedDefinition kDef_C_ZB_RD1{
    .zigbee_models=kModels_C_ZB_RD1, .zigbee_models_count=sizeof(kModels_C_ZB_RD1)/sizeof(kModels_C_ZB_RD1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="C-ZB-RD1", .vendor="Candeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C_ZB_RD1, .from_zigbee_count=sizeof(kFz_C_ZB_RD1)/sizeof(kFz_C_ZB_RD1[0]),
    .to_zigbee=kTz_C_ZB_RD1, .to_zigbee_count=sizeof(kTz_C_ZB_RD1)/sizeof(kTz_C_ZB_RD1[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::candeo
