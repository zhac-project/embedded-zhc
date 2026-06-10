// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Essentielb EB-E27-A60-RGBW — hand-rewritten colour light.
// E27 A60 RGBW light bulb
// z2m extend: m.light({colorTemp: {range: [153, 370]}, color: true})
//   -> on/off + level + colorTemp + color. The auto-generator
//   collapsed m.light() to on/off + brightness only, dropping the
//   CT + color axes; this file restores kFzColorTemperature +
//   kFzColor (and their TZ) + the colour exposes + 0x0300 binding.
// z2m-source: essentialb.ts #EB-E27-A60-RGBW.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::essentialb {
namespace {
const FzConverter* const kFz_EB_E27_A60_RGBW[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_EB_E27_A60_RGBW[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_EB_E27_A60_RGBW[] = { "EB-E27-A60-RGBW" };

}  // namespace


// Hand-curated colour-light surface.
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},  // lightingColorCtrl (CT + color)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_EB_E27_A60_RGBW{
    .zigbee_models=kModels_EB_E27_A60_RGBW, .zigbee_models_count=sizeof(kModels_EB_E27_A60_RGBW)/sizeof(kModels_EB_E27_A60_RGBW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EB-E27-A60-RGBW", .vendor="Essentialb",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_EB_E27_A60_RGBW, .from_zigbee_count=sizeof(kFz_EB_E27_A60_RGBW)/sizeof(kFz_EB_E27_A60_RGBW[0]),
    .to_zigbee=kTz_EB_E27_A60_RGBW, .to_zigbee_count=sizeof(kTz_EB_E27_A60_RGBW)/sizeof(kTz_EB_E27_A60_RGBW[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::essentialb
