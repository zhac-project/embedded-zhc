// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Light Solutions 5-in-1 RGBCCT LED controller (z2m v26.92.0 parity).
// z2m-source: lightsolutions.ts #91-943-PRO-RGBCW (added v26.86.0)
// Upstream colorTemp range [158, 500] mireds.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lightsolutions {
namespace {
const FzConverter* const kFz_D91_943_PRO_RGBCW[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_D91_943_PRO_RGBCW[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_D91_943_PRO_RGBCW[] = { "91-943-PRO-RGBCW" };

constexpr Expose kExposes_D91_943_PRO_RGBCW[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_D91_943_PRO_RGBCW[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_D91_943_PRO_RGBCW{
    .zigbee_models=kModels_D91_943_PRO_RGBCW, .zigbee_models_count=sizeof(kModels_D91_943_PRO_RGBCW)/sizeof(kModels_D91_943_PRO_RGBCW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="91-943-PRO-RGBCW", .vendor="Light Solutions",
    .meta=nullptr, .exposes=kExposes_D91_943_PRO_RGBCW, .exposes_count=sizeof(kExposes_D91_943_PRO_RGBCW)/sizeof(kExposes_D91_943_PRO_RGBCW[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D91_943_PRO_RGBCW, .from_zigbee_count=sizeof(kFz_D91_943_PRO_RGBCW)/sizeof(kFz_D91_943_PRO_RGBCW[0]),
    .to_zigbee=kTz_D91_943_PRO_RGBCW, .to_zigbee_count=sizeof(kTz_D91_943_PRO_RGBCW)/sizeof(kTz_D91_943_PRO_RGBCW[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D91_943_PRO_RGBCW, .bindings_count=sizeof(kBindings_D91_943_PRO_RGBCW)/sizeof(kBindings_D91_943_PRO_RGBCW[0]),
};

}  // namespace zhc::devices::lightsolutions
