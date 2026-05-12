// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Candeo C-ZB-RD1P-DIM.
// Zigbee rotary dimmer pro (dimmer mode)
// z2m-source: candeo.ts #C-ZB-RD1P-DIM.
#include "definitions/_generic/_shared.hpp"


namespace zhc::devices::candeo {
namespace {
const FzConverter* const kFz_C_ZB_RD1P_DIM[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzPowerOnBehavior1
};
const TzConverter* const kTz_C_ZB_RD1P_DIM[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzPowerOnBehavior1
};
constexpr const char* kModels_C_ZB_RD1P_DIM[] = { "C-ZB-RD1P-DIM" };

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

extern const PreparedDefinition kDef_C_ZB_RD1P_DIM{
    .zigbee_models=kModels_C_ZB_RD1P_DIM, .zigbee_models_count=sizeof(kModels_C_ZB_RD1P_DIM)/sizeof(kModels_C_ZB_RD1P_DIM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="C-ZB-RD1P-DIM", .vendor="Candeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C_ZB_RD1P_DIM, .from_zigbee_count=sizeof(kFz_C_ZB_RD1P_DIM)/sizeof(kFz_C_ZB_RD1P_DIM[0]),
    .to_zigbee=kTz_C_ZB_RD1P_DIM, .to_zigbee_count=sizeof(kTz_C_ZB_RD1P_DIM)/sizeof(kTz_C_ZB_RD1P_DIM[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::candeo
