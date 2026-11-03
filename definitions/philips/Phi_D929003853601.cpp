// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Philips 929003853601 Hue white and color ambiance A67 1600lm E27 (z2m v26.125.0 window).
// philips.m.light({colorTemp: [50, 1000], color: xy + hs, enhancedHue}) + m.identify().
// z2m-source: philips.ts #929003853601
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::philips {
namespace {
const FzConverter* const kFz_D929003853601[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_D929003853601[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_D929003853601[] = { "LCA022" };

constexpr Expose kExposes_D929003853601[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0, ExposeCategory::State, 50, 1000, 1},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_D929003853601[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_D929003853601{
    .zigbee_models=kModels_D929003853601, .zigbee_models_count=sizeof(kModels_D929003853601)/sizeof(kModels_D929003853601[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="929003853601", .vendor="Philips",
    .meta=nullptr, .exposes=kExposes_D929003853601, .exposes_count=sizeof(kExposes_D929003853601)/sizeof(kExposes_D929003853601[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D929003853601, .from_zigbee_count=sizeof(kFz_D929003853601)/sizeof(kFz_D929003853601[0]),
    .to_zigbee=kTz_D929003853601, .to_zigbee_count=sizeof(kTz_D929003853601)/sizeof(kTz_D929003853601[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D929003853601, .bindings_count=sizeof(kBindings_D929003853601)/sizeof(kBindings_D929003853601[0]),
};

}  // namespace zhc::devices::philips
