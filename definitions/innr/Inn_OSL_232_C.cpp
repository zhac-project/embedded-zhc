// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Innr OSL 232 C LED strip (z2m v26.76.0 parity).
// z2m-source: innr.ts #OSL 232 C
// Outdoor Smart Spot White & Colour — m.light({colorTemp:{range:[50,1000]},
// color:{modes:["xy","hs"], enhancedHue:true}}): on/off + brightness +
// color_temp (mired) + color (xy + hs). Structure mirrors awox ColorCTLight.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::innr {
namespace {
const FzConverter* const kFz_OSL_232_C[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_OSL_232_C[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_OSL_232_C[] = { "OSL 232 C" };

constexpr Expose kExposes_OSL_232_C[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_OSL_232_C[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_OSL_232_C{
    .zigbee_models=kModels_OSL_232_C, .zigbee_models_count=sizeof(kModels_OSL_232_C)/sizeof(kModels_OSL_232_C[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="OSL 232 C", .vendor="Innr",
    .meta=nullptr, .exposes=kExposes_OSL_232_C, .exposes_count=sizeof(kExposes_OSL_232_C)/sizeof(kExposes_OSL_232_C[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_OSL_232_C, .from_zigbee_count=sizeof(kFz_OSL_232_C)/sizeof(kFz_OSL_232_C[0]),
    .to_zigbee=kTz_OSL_232_C, .to_zigbee_count=sizeof(kTz_OSL_232_C)/sizeof(kTz_OSL_232_C[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_OSL_232_C, .bindings_count=sizeof(kBindings_OSL_232_C)/sizeof(kBindings_OSL_232_C[0]),
};

}  // namespace zhc::devices::innr
