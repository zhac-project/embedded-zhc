// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Candeo C-ZB-LC20-RGBW.
// Smart LED controller (RGBW mode)
// z2m-source: candeo.ts #C-ZB-LC20-RGBW.
#include "definitions/_generic/_shared.hpp"


namespace zhc::devices::candeo {
namespace {
const FzConverter* const kFz_C_ZB_LC20_RGBW[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzPowerOnBehavior1
};
const TzConverter* const kTz_C_ZB_LC20_RGBW[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzPowerOnBehavior1
};
constexpr const char* kModels_C_ZB_LC20_RGBW[] = { "C-ZB-LC20-RGBW", "C-ZB-LC20v2-RGBW" };

constexpr Expose kAutoExposes[] = {
    {"state",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_x",           ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y",           ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue",               ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp",        ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0}
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300}
};

}  // namespace

extern const PreparedDefinition kDef_C_ZB_LC20_RGBW{
    .zigbee_models=kModels_C_ZB_LC20_RGBW, .zigbee_models_count=sizeof(kModels_C_ZB_LC20_RGBW)/sizeof(kModels_C_ZB_LC20_RGBW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="C-ZB-LC20-RGBW", .vendor="Candeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C_ZB_LC20_RGBW, .from_zigbee_count=sizeof(kFz_C_ZB_LC20_RGBW)/sizeof(kFz_C_ZB_LC20_RGBW[0]),
    .to_zigbee=kTz_C_ZB_LC20_RGBW, .to_zigbee_count=sizeof(kTz_C_ZB_LC20_RGBW)/sizeof(kTz_C_ZB_LC20_RGBW[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::candeo
