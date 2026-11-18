// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Philips 929003858301 Hue Festavia globe outdoor string lights (7 m, 8 bulbs) (z2m v26.133.0 window).
// z2m: philips.m.light({colorTemp: {range: [50, 1000]}, color: true, gradient: true}) + m.identify().
// Same surface as 929004320801 with this colour-temperature range; the gradient
// segments and identify are not ported, as there.
// z2m-source: philips.ts #929003858301
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::philips {
namespace {
const FzConverter* const kFz_D929003858301[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_D929003858301[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_D929003858301[] = { "LCX026" };

constexpr Expose kExposes_D929003858301[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0, ExposeCategory::State, 50, 1000, 1},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_D929003858301[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_D929003858301{
    .zigbee_models=kModels_D929003858301, .zigbee_models_count=sizeof(kModels_D929003858301)/sizeof(kModels_D929003858301[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="929003858301", .vendor="Philips",
    .meta=nullptr, .exposes=kExposes_D929003858301, .exposes_count=sizeof(kExposes_D929003858301)/sizeof(kExposes_D929003858301[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D929003858301, .from_zigbee_count=sizeof(kFz_D929003858301)/sizeof(kFz_D929003858301[0]),
    .to_zigbee=kTz_D929003858301, .to_zigbee_count=sizeof(kTz_D929003858301)/sizeof(kTz_D929003858301[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D929003858301, .bindings_count=sizeof(kBindings_D929003858301)/sizeof(kBindings_D929003858301[0]),
};

}  // namespace zhc::devices::philips
