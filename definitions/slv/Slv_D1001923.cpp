// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Slv 1001923 — graduated override (color + color_temp restored).
// VALETO LED GU10 RGBW
// z2m-source: slv.ts #1001923 — m.light({colorTemp:{range:[153,556]}, color:true}).
// Gap: auto-port dropped both the color (color_x/color_y) and color_temp
// axes (lightingColorCtrl 0x0300); it carried only on/off + brightness.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::slv {
namespace {
const FzConverter* const kFz_D1001923[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_D1001923[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_D1001923[] = { "ZBT-RGBWLight-AR0844" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_xy", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

extern const PreparedDefinition kDef_D1001923{
    .zigbee_models=kModels_D1001923, .zigbee_models_count=sizeof(kModels_D1001923)/sizeof(kModels_D1001923[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="1001923", .vendor="Slv",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D1001923, .from_zigbee_count=sizeof(kFz_D1001923)/sizeof(kFz_D1001923[0]),
    .to_zigbee=kTz_D1001923, .to_zigbee_count=sizeof(kTz_D1001923)/sizeof(kTz_D1001923[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::slv
