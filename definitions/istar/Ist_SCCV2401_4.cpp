// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Istar SCCV2401-4 — hand-rewritten RGBW light bundle.
// Zigbee 3.0 LED controller, dimmable RGBW.
// z2m extend: m.light({colorTemp: {range: [275, 295]},
//                      color: {modes: ["xy", "hs"], enhancedHue: true}})
//   → on/off + level + colorTemp + color (xy + hs). The auto-port
//   collapsed m.light() to on/off + brightness only, dropping the
//   colorTemp AND the full colour channel. Restored kFzColorTemperature
//   + kFzColor (and tz peers), the color_temp / color_x / color_y / hue /
//   saturation exposes, and the 0x0300 binding.
// z2m-source: istar.ts #SCCV2401-4.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::istar {
namespace {
const FzConverter* const kFz_SCCV2401_4[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_SCCV2401_4[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_SCCV2401_4[] = { "iStar RGBW Light" };

}  // namespace


// Hand-curated ColorCTLight surface (CT range 275–295 mired per z2m).
constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0, ExposeCategory::State, 275, 295, 0},
    {"color_x",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (CT + color)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_SCCV2401_4{
    .zigbee_models=kModels_SCCV2401_4, .zigbee_models_count=sizeof(kModels_SCCV2401_4)/sizeof(kModels_SCCV2401_4[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SCCV2401-4", .vendor="Istar",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SCCV2401_4, .from_zigbee_count=sizeof(kFz_SCCV2401_4)/sizeof(kFz_SCCV2401_4[0]),
    .to_zigbee=kTz_SCCV2401_4, .to_zigbee_count=sizeof(kTz_SCCV2401_4)/sizeof(kTz_SCCV2401_4[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::istar
