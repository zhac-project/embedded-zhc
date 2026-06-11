// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: AcuityBrandsLighting RB56SC — graduated for a parity fix.
// Juno Retrobasics 4" and 6" LED smart downlight (RGB + tunable white).
// z2m-source: acuity_brands_lighting.ts #RB56SC.
//
// z2m bundle:
//   m.light({colorTemp: {range: [200, 370], startup: false}, color: true}).
//   colorTemp + color both add the lightingColorCtrl (0x0300) cluster:
//   a color_temp channel (mireds, 200-370) and the full colour (xy) axis.
//   The auto-port wired only kFzOnOff + kFzBrightness and exposed
//   state + brightness, dropping the whole 0x0300 channel. Restored
//   kFzColorTemperature + kFzColor (decode) / kTzColorTemp + kTzColor
//   (control), the color_temp / color_x / color_y exposes, and the 0x0300
//   binding.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::acuity_brands_lighting {
namespace {
const FzConverter* const kFz_RB56SC[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_RB56SC[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_RB56SC[] = { "ABL-LIGHT-Z-201" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0, ExposeCategory::State, 200, 370, 0},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

extern const PreparedDefinition kDef_RB56SC{
    .zigbee_models=kModels_RB56SC, .zigbee_models_count=sizeof(kModels_RB56SC)/sizeof(kModels_RB56SC[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RB56SC", .vendor="AcuityBrandsLighting",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_RB56SC, .from_zigbee_count=sizeof(kFz_RB56SC)/sizeof(kFz_RB56SC[0]),
    .to_zigbee=kTz_RB56SC, .to_zigbee_count=sizeof(kTz_RB56SC)/sizeof(kTz_RB56SC[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::acuity_brands_lighting
