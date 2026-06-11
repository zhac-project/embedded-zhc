// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lds ZBT-RGBWLight-A0000 — hand-rewritten RGBW light bundle.
// Ynoa smart LED E27. z2m extend = m.light({colorTemp:{range:[153,555]},
// color:true}) → on/off + brightness + colorTemp (mireds) + color (default
// modes xy + hs). The auto-generator collapsed this to on/off + brightness
// only, dropping the entire color + colorTemp axis; this rewrite restores it
// (kFzColor + kFzColorTemperature / kTzColor + kTzColorTemp, bind 0x0300).
// z2m-source: lds.ts #ZBT-RGBWLight-A0000.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lds {
namespace {
const FzConverter* const kFz_ZBT_RGBWLight_A0000[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_ZBT_RGBWLight_A0000[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_ZBT_RGBWLight_A0000[] = { "ZBT-RGBWLight-A0000" };

constexpr Expose kExposes_ZBT_RGBWLight_A0000[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired",  nullptr, nullptr, 0},
    {"color_x",    ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"color_y",    ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"hue",        ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ZBT_RGBWLight_A0000[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (CT + color)
};

}  // namespace

extern const PreparedDefinition kDef_ZBT_RGBWLight_A0000{
    .zigbee_models=kModels_ZBT_RGBWLight_A0000, .zigbee_models_count=sizeof(kModels_ZBT_RGBWLight_A0000)/sizeof(kModels_ZBT_RGBWLight_A0000[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZBT-RGBWLight-A0000", .vendor="Lds",
    .meta=nullptr, .exposes=kExposes_ZBT_RGBWLight_A0000, .exposes_count=sizeof(kExposes_ZBT_RGBWLight_A0000)/sizeof(kExposes_ZBT_RGBWLight_A0000[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZBT_RGBWLight_A0000, .from_zigbee_count=sizeof(kFz_ZBT_RGBWLight_A0000)/sizeof(kFz_ZBT_RGBWLight_A0000[0]),
    .to_zigbee=kTz_ZBT_RGBWLight_A0000, .to_zigbee_count=sizeof(kTz_ZBT_RGBWLight_A0000)/sizeof(kTz_ZBT_RGBWLight_A0000[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZBT_RGBWLight_A0000, .bindings_count=sizeof(kBindings_ZBT_RGBWLight_A0000)/sizeof(kBindings_ZBT_RGBWLight_A0000[0]),
};

}  // namespace zhc::devices::lds
