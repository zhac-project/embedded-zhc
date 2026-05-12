// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Trust ZLED-RGB9 — hand-rewritten for parity (full CT+color, EP 2 default).
// Smart RGB LED bulb (m.light({colorTemp:[153,500], color:true,
//                              powerOnBehavior:false}); endpoint default = 2).
// z2m-source: trust.ts #ZLED-RGB9.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::trust {
namespace {
const FzConverter* const kFz_ZLED_RGB9[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_ZLED_RGB9[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_ZLED_RGB9[] = { "ZLL-ExtendedColo" };

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
    {2, 0x0006},
    {2, 0x0008},
    {2, 0x0300},
};

extern const PreparedDefinition kDef_ZLED_RGB9{
    .zigbee_models=kModels_ZLED_RGB9, .zigbee_models_count=sizeof(kModels_ZLED_RGB9)/sizeof(kModels_ZLED_RGB9[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZLED-RGB9", .vendor="Trust",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZLED_RGB9, .from_zigbee_count=sizeof(kFz_ZLED_RGB9)/sizeof(kFz_ZLED_RGB9[0]),
    .to_zigbee=kTz_ZLED_RGB9, .to_zigbee_count=sizeof(kTz_ZLED_RGB9)/sizeof(kTz_ZLED_RGB9[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .default_endpoint=2,
};

}  // namespace zhc::devices::trust
