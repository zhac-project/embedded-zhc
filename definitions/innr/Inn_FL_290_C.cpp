// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Innr FL 290 C, lightstrip colour 10 m, new in z2m
// v26.126.0..v26.133.0.
// m.light({colorTemp: {range: [50, 1000]}, color: {modes: ["xy", "hs"],
// enhancedHue: true}}): state, brightness, color_temp 50-1000 mired, colour
// (xy + hs, kFzColor / kTzColor), color_mode, effect (+ colorloop),
// power_on_behavior. Written in full rather than copied from the FL 250 C
// generated def, which carries on/off + brightness only.
// Not ported: color_temp_startup; enhancedHue (hs writes go out as
// moveToHueAndSaturation, not the enhanced command).
// z2m-source: innr.ts #FL 290 C.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::innr {
namespace {

const FzConverter* const kFz_FL_290_C[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzPowerOnBehavior1,
};
const TzConverter* const kTz_FL_290_C[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzPowerOnBehavior1,
    &::zhc::generic::kTzEffect,
    &::zhc::generic::kTzEffectColorLoop,
};

constexpr const char* kModels_FL_290_C[] = { "FL 290 C" };

constexpr const char* kEffects[] = {
    "blink", "breathe", "okay", "channel_change", "finish_effect", "stop_effect",
    "colorloop", "stop_colorloop" };
constexpr const char* kPowerOn[] = { "off", "on", "toggle", "previous" };

constexpr Expose kExposes_FL_290_C[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0,
     ExposeCategory::State, 50, 1000, 1},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_mode", ExposeType::Enum, Access::State, nullptr, nullptr, ::zhc::generic::kColorModes,
     sizeof(::zhc::generic::kColorModes)/sizeof(::zhc::generic::kColorModes[0])},
    {"effect", ExposeType::Enum, Access::Set, nullptr, "Triggers an effect on the light",
     kEffects, sizeof(kEffects)/sizeof(kEffects[0])},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr,
     kPowerOn, sizeof(kPowerOn)/sizeof(kPowerOn[0])},
};

constexpr BindingSpec kBindings_FL_290_C[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_FL_290_C{
    .zigbee_models=kModels_FL_290_C, .zigbee_models_count=sizeof(kModels_FL_290_C)/sizeof(kModels_FL_290_C[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="FL 290 C", .vendor="Innr",
    .meta=nullptr, .exposes=kExposes_FL_290_C, .exposes_count=sizeof(kExposes_FL_290_C)/sizeof(kExposes_FL_290_C[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_FL_290_C, .from_zigbee_count=sizeof(kFz_FL_290_C)/sizeof(kFz_FL_290_C[0]),
    .to_zigbee=kTz_FL_290_C, .to_zigbee_count=sizeof(kTz_FL_290_C)/sizeof(kTz_FL_290_C[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_FL_290_C, .bindings_count=sizeof(kBindings_FL_290_C)/sizeof(kBindings_FL_290_C[0]),
};

}  // namespace zhc::devices::innr
