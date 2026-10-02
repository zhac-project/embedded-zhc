// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Philips 929004320801 Hue Play floor lamp large (z2m v26.115.1 window).
// Same surface as 929004321001: philips.m.light({colorTemp: [153, 500],
// color, gradient}) + m.identify(); the gradient segments are not ported here
// either.
// z2m-source: philips.ts #929004320801
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::philips {
namespace {
const FzConverter* const kFz_D929004320801[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_D929004320801[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_D929004320801[] = { "929004320801" };

constexpr Expose kExposes_D929004320801[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0, ExposeCategory::State, 153, 500, 1},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_D929004320801[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_D929004320801{
    .zigbee_models=kModels_D929004320801, .zigbee_models_count=sizeof(kModels_D929004320801)/sizeof(kModels_D929004320801[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="929004320801", .vendor="Philips",
    .meta=nullptr, .exposes=kExposes_D929004320801, .exposes_count=sizeof(kExposes_D929004320801)/sizeof(kExposes_D929004320801[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D929004320801, .from_zigbee_count=sizeof(kFz_D929004320801)/sizeof(kFz_D929004320801[0]),
    .to_zigbee=kTz_D929004320801, .to_zigbee_count=sizeof(kTz_D929004320801)/sizeof(kTz_D929004320801[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D929004320801, .bindings_count=sizeof(kBindings_D929004320801)/sizeof(kBindings_D929004320801[0]),
};

}  // namespace zhc::devices::philips
