// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Zipato rgbw2.zbee27 — hand-rewritten for parity (full color + colorTemp).
// RGBW LED bulb with dimmer. z2m = m.light({colorTemp:{range:undefined},
// color:true}) → on/off + brightness + colorTemp (mireds) + color (xy + hs).
// The auto-port collapsed it to on/off + brightness only, dropping the colour
// and colour-temperature axes; restored kFzColorTemperature + kFzColor
// (+ kTzColorTemp / kTzColor), color_temp/color_x/color_y/hue/saturation
// exposes, and bind 0x0300 lightingColorCtrl.
// z2m-source: zipato.ts #rgbw2.zbee27.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::zipato {
namespace {
const FzConverter* const kFz_rgbw2_zbee27[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_rgbw2_zbee27[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_rgbw2_zbee27[] = { "ZHA-ColorLight" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_x",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

extern const PreparedDefinition kDef_rgbw2_zbee27{
    .zigbee_models=kModels_rgbw2_zbee27, .zigbee_models_count=sizeof(kModels_rgbw2_zbee27)/sizeof(kModels_rgbw2_zbee27[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="rgbw2.zbee27", .vendor="Zipato",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_rgbw2_zbee27, .from_zigbee_count=sizeof(kFz_rgbw2_zbee27)/sizeof(kFz_rgbw2_zbee27[0]),
    .to_zigbee=kTz_rgbw2_zbee27, .to_zigbee_count=sizeof(kTz_rgbw2_zbee27)/sizeof(kTz_rgbw2_zbee27[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::zipato
