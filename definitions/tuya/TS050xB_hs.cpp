// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya hue/saturation-only colour lights split out upstream in the
// z2m v26.125.0 window, as manufacturer overrides of the model-only defs:
//   TS0505B_hs  TS0505B + _TZ3210_ffuna0nr                   (RGBCW bulb)
//   TS0503B_hs  TS0503B + _TZ3210_rbixajyp, _TZ3210_w7ge4ldo (RGB COB strip)
// tuyaLight({color: {modes: ["hs"]}}, + colorTemp 153-500 on TS0505B_hs):
// the same converters as TS0505B (Tuya 0xF000 brightness, effect,
// do_not_disturb, color_power_on_behavior), exposing hue/saturation instead
// of xy. kTzColor already sends moveToHueAndSaturation for hue/saturation
// input. Before this the bulb landed on the xy-only TS0505B and the strip on
// TS0503B, which has no colour control at all.
// z2m-source: tuya.ts #TS0505B_hs #TS0503B_hs.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
namespace zhc::devices::tuya {
namespace {
const FzConverter* const kFzRgbcw[] = {
    &::zhc::generic::kFzOnOff, &::zhc::generic::kFzBrightness,
    &::zhc::tuya::kFzTuyaBrightness,
    &::zhc::generic::kFzColorTemperature, &::zhc::generic::kFzColor };
const TzConverter* const kTzRgbcw[] = {
    &::zhc::generic::kTzOnOff, &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp, &::zhc::generic::kTzColor,
    &::zhc::generic::kTzEffect, &::zhc::generic::kTzEffectColorLoop,
    &::zhc::tuya::kTzTuyaDoNotDisturb, &::zhc::tuya::kTzTuyaColorPowerOnBehavior };
const FzConverter* const kFzRgb[] = {
    &::zhc::generic::kFzOnOff, &::zhc::generic::kFzBrightness,
    &::zhc::tuya::kFzTuyaBrightness, &::zhc::generic::kFzColor };
const TzConverter* const kTzRgb[] = {
    &::zhc::generic::kTzOnOff, &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzEffect, &::zhc::generic::kTzEffectColorLoop,
    &::zhc::tuya::kTzTuyaDoNotDisturb, &::zhc::tuya::kTzTuyaColorPowerOnBehavior };

constexpr const char* kEffects[] = {
    "blink", "breathe", "okay", "channel_change", "finish_effect", "stop_effect",
    "colorloop", "stop_colorloop" };
constexpr const char* kColorPowerOn[] = { "initial", "previous", "customized" };

constexpr Expose kExposesRgbcw[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0,
     ExposeCategory::State, 153, 500, 1},
    {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_mode", ExposeType::Enum, Access::State, nullptr, nullptr, ::zhc::generic::kColorModes, 3},
    {"effect", ExposeType::Enum, Access::Set, nullptr, "Triggers an effect on the light",
     kEffects, 8},
    {"do_not_disturb", ExposeType::Binary, Access::Set, nullptr, nullptr, nullptr, 0,
     ExposeCategory::Config},
    {"color_power_on_behavior", ExposeType::Enum, Access::Set, nullptr, "Power on behavior state",
     kColorPowerOn, 3, ExposeCategory::Config},
};
constexpr Expose kExposesRgb[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_mode", ExposeType::Enum, Access::State, nullptr, nullptr, ::zhc::generic::kColorModes, 3},
    {"effect", ExposeType::Enum, Access::Set, nullptr, "Triggers an effect on the light",
     kEffects, 8},
    {"do_not_disturb", ExposeType::Binary, Access::Set, nullptr, nullptr, nullptr, 0,
     ExposeCategory::Config},
    {"color_power_on_behavior", ExposeType::Enum, Access::Set, nullptr, "Power on behavior state",
     kColorPowerOn, 3, ExposeCategory::Config},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

constexpr const char* kModelsRgbcw[] = { "TS0505B" };
constexpr const char* kManusRgbcw[] = { "_TZ3210_ffuna0nr" };
constexpr const char* kModelsRgb[] = { "TS0503B" };
constexpr const char* kManusRgb[] = { "_TZ3210_rbixajyp", "_TZ3210_w7ge4ldo" };
}  // namespace

extern const PreparedDefinition kDefTS0505B_hs{
    .zigbee_models=kModelsRgbcw,.zigbee_models_count=sizeof(kModelsRgbcw)/sizeof(kModelsRgbcw[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManusRgbcw,.manufacturer_names_count=sizeof(kManusRgbcw)/sizeof(kManusRgbcw[0]),
    .model="TS0505B_hs",.vendor="Tuya",
    .meta=nullptr,.exposes=kExposesRgbcw,.exposes_count=sizeof(kExposesRgbcw)/sizeof(kExposesRgbcw[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFzRgbcw,.from_zigbee_count=sizeof(kFzRgbcw)/sizeof(kFzRgbcw[0]),
    .to_zigbee=kTzRgbcw,.to_zigbee_count=sizeof(kTzRgbcw)/sizeof(kTzRgbcw[0]),
    .configure=nullptr,.on_event=nullptr,
    .bindings=kBindings,.bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    // Kept from the TS0505B def it resolved to before (kReportsLightRGBCCT_1ep).
    .reports=::zhc::tuya::kReportsLightRGBCCT_1ep,
    .reports_count=::zhc::tuya::kReportsLightRGBCCT_1ep_count,
};

extern const PreparedDefinition kDefTS0503B_hs{
    .zigbee_models=kModelsRgb,.zigbee_models_count=sizeof(kModelsRgb)/sizeof(kModelsRgb[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManusRgb,.manufacturer_names_count=sizeof(kManusRgb)/sizeof(kManusRgb[0]),
    .model="TS0503B_hs",.vendor="Tuya",
    .meta=nullptr,.exposes=kExposesRgb,.exposes_count=sizeof(kExposesRgb)/sizeof(kExposesRgb[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFzRgb,.from_zigbee_count=sizeof(kFzRgb)/sizeof(kFzRgb[0]),
    .to_zigbee=kTzRgb,.to_zigbee_count=sizeof(kTzRgb)/sizeof(kTzRgb[0]),
    .configure=nullptr,.on_event=nullptr,
    .bindings=kBindings,.bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
};
}  // namespace zhc::devices::tuya
