// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: eWeLink CK-TLSR8258-L5PI-01(7009) 18 W RGBCW E27 bulb (z2m v26.115.1 window).
// z2m m.light({colorTemp: [153, 370], effect, powerOnBehavior, color: xy/hs}).
// z2m-source: tuya.ts #CK-TLSR8258-L5PI-01(7009).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::tuya {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzPowerOnBehavior1,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzEffect,
    &::zhc::generic::kTzEffectColorLoop,
    &::zhc::generic::kTzPowerOnBehavior1,
};
constexpr const char* kOpts7[] = { "blink", "breathe", "okay", "channel_change", "finish_effect", "stop_effect", "colorloop", "stop_colorloop" };
constexpr const char* kOpts8[] = { "off", "on", "toggle", "previous" };
constexpr Expose kExp[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0, ExposeCategory::State, 153, 370, 1},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"effect", ExposeType::Enum, Access::Set, nullptr, nullptr, kOpts7, 8},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, "Controls the behavior when the device is powered on after power loss", kOpts8, 4, ExposeCategory::Config},
};
constexpr const char* kM[] = { "CK-TLSR8258-L5PI-01(7009)" };
constexpr BindingSpec kBind[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};
}  // namespace

extern const PreparedDefinition kDef_CK_TLSR8258_L5PI_01_7009{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CK-TLSR8258-L5PI-01(7009)", .vendor="eWeLink",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
};

}  // namespace zhc::devices::tuya
