// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: AwoX / EGLO Connect-Z LED G95 13.5W RGB + tunable white (z2m v26.92.0 parity).
// z2m-source: awox.ts #EZMB-RGB-TW-I2C (added v26.85.0)
// Upstream colorTemp range [153, 370] mireds.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::awox {
namespace {
const FzConverter* const kFz_EZMB_RGB_TW_I2C[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_EZMB_RGB_TW_I2C[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_EZMB_RGB_TW_I2C[] = { "EZMB-RGB-TW-I2C" };

constexpr Expose kExposes_EZMB_RGB_TW_I2C[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_EZMB_RGB_TW_I2C[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

constexpr WhiteLabel kWhiteLabels_EZMB_RGB_TW_I2C[] = {
    {"EGLO", "12254"},
};

}  // namespace

extern const PreparedDefinition kDef_EZMB_RGB_TW_I2C{
    .zigbee_models=kModels_EZMB_RGB_TW_I2C, .zigbee_models_count=sizeof(kModels_EZMB_RGB_TW_I2C)/sizeof(kModels_EZMB_RGB_TW_I2C[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EZMB-RGB-TW-I2C", .vendor="AwoX",
    .meta=nullptr, .exposes=kExposes_EZMB_RGB_TW_I2C, .exposes_count=sizeof(kExposes_EZMB_RGB_TW_I2C)/sizeof(kExposes_EZMB_RGB_TW_I2C[0]),
    .white_labels=kWhiteLabels_EZMB_RGB_TW_I2C, .white_labels_count=sizeof(kWhiteLabels_EZMB_RGB_TW_I2C)/sizeof(kWhiteLabels_EZMB_RGB_TW_I2C[0]),
    .from_zigbee=kFz_EZMB_RGB_TW_I2C, .from_zigbee_count=sizeof(kFz_EZMB_RGB_TW_I2C)/sizeof(kFz_EZMB_RGB_TW_I2C[0]),
    .to_zigbee=kTz_EZMB_RGB_TW_I2C, .to_zigbee_count=sizeof(kTz_EZMB_RGB_TW_I2C)/sizeof(kTz_EZMB_RGB_TW_I2C[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_EZMB_RGB_TW_I2C, .bindings_count=sizeof(kBindings_EZMB_RGB_TW_I2C)/sizeof(kBindings_EZMB_RGB_TW_I2C[0]),
};

}  // namespace zhc::devices::awox
