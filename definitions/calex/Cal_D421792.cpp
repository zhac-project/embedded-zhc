// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Calex 421792 — graduated from generated/ for a parity fix.
// LED A60 Zigbee RGB lamp.
//
// Parity fix (was on/off + brightness only): z2m drives this with
// `m.light({colorTemp: {range: [153, 370]}, color: {modes: ["xy", "hs"]}})`,
// which exposes the full colour axis (color_xy + color_hs) AND color_temp.
// The auto-port dropped the entire lightingColorCtrl (0x0300) channel — no
// hue/saturation/color_x/color_y/color_temp decode, no colour toZigbee path,
// no 0x0300 binding. Restored kFzColor + kFzColorTemperature (and the matching
// kTzColor + kTzColorTemp) plus the colour exposes and the 0x0300 binding.
// z2m-source: calex.ts #421792.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::calex {
namespace {
const FzConverter* const kFz_D421792[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,             // lightingColorCtrl hue/sat/xy/color_mode
    &::zhc::generic::kFzColorTemperature,  // lightingColorCtrl attr 0x0007 → color_temp
};
const TzConverter* const kTz_D421792[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_D421792[] = { "EC-Z3.0-RGBW" };

constexpr Expose kExposes_D421792[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    // colour: modes ["xy","hs"] → hue/saturation + color_x/color_y.
    {"hue",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_x",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    // colorTemp range [153, 370] mireds.
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_D421792[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (colour + colour temperature)
};

}  // namespace


extern const PreparedDefinition kDef_D421792{
    .zigbee_models=kModels_D421792, .zigbee_models_count=sizeof(kModels_D421792)/sizeof(kModels_D421792[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="421792", .vendor="Calex",
    .meta=nullptr, .exposes=kExposes_D421792, .exposes_count=sizeof(kExposes_D421792)/sizeof(kExposes_D421792[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D421792, .from_zigbee_count=sizeof(kFz_D421792)/sizeof(kFz_D421792[0]),
    .to_zigbee=kTz_D421792, .to_zigbee_count=sizeof(kTz_D421792)/sizeof(kTz_D421792[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kBindings_D421792,.bindings_count=sizeof(kBindings_D421792)/sizeof(kBindings_D421792[0]),
};

}  // namespace zhc::devices::calex
