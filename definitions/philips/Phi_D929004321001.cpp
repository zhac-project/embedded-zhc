// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Philips Hue Play floor lamp large (z2m v26.92.0 parity).
// z2m-source: philips.ts #929004321001 (added v26.89.0)
// Upstream colorTemp range [153, 454] mireds.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::philips {
namespace {
const FzConverter* const kFz_D929004321001[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_D929004321001[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_D929004321001[] = { "929004321001" };

constexpr Expose kExposes_D929004321001[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_D929004321001[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_D929004321001{
    .zigbee_models=kModels_D929004321001, .zigbee_models_count=sizeof(kModels_D929004321001)/sizeof(kModels_D929004321001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="929004321001", .vendor="Philips",
    .meta=nullptr, .exposes=kExposes_D929004321001, .exposes_count=sizeof(kExposes_D929004321001)/sizeof(kExposes_D929004321001[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D929004321001, .from_zigbee_count=sizeof(kFz_D929004321001)/sizeof(kFz_D929004321001[0]),
    .to_zigbee=kTz_D929004321001, .to_zigbee_count=sizeof(kTz_D929004321001)/sizeof(kTz_D929004321001[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D929004321001, .bindings_count=sizeof(kBindings_D929004321001)/sizeof(kBindings_D929004321001[0]),
};

}  // namespace zhc::devices::philips
