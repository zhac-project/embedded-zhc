// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Jiawen K2RGBW01 — hand-rewritten RGBW (color + colorTemp) bundle.
// Wireless Bulb E27 9W RGBW
// z2m extend: m.light({colorTemp: {range: undefined}, color: true})
//   → on/off + level + colorTemp (mireds) + xy color (RGBW bulb).
// The auto-generator collapses m.light() to on/off + brightness only,
// dropping BOTH the colorTemp and the xy-color axes — this file restores them.
// z2m-source: jiawen.ts #K2RGBW01.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::jiawen {
namespace {
const FzConverter* const kFz_K2RGBW01[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_K2RGBW01[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_K2RGBW01[] = { "FB56-ZCW08KU1.1", "FB56-ZCW08KU1.0" };

}  // namespace


// Hand-curated RGBW light surface: on/off + brightness + colorTemp + xy color.
constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_xy",   ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (colorTemp + xy — was dropped)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_K2RGBW01{
    .zigbee_models=kModels_K2RGBW01, .zigbee_models_count=sizeof(kModels_K2RGBW01)/sizeof(kModels_K2RGBW01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="K2RGBW01", .vendor="Jiawen",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_K2RGBW01, .from_zigbee_count=sizeof(kFz_K2RGBW01)/sizeof(kFz_K2RGBW01[0]),
    .to_zigbee=kTz_K2RGBW01, .to_zigbee_count=sizeof(kTz_K2RGBW01)/sizeof(kTz_K2RGBW01[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::jiawen
