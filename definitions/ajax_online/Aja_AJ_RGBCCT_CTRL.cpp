// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: AjaxOnline AJ_RGBCCT_CTRL — hand-rewritten for parity (full colour).
// Smart Zigbee LED strip RGB+CCT
// z2m-source: ajax_online.ts #AJ_RGBCCT_CTRL.
//
// z2m bundle: tuya.modernExtend.tuyaLight({color: true,
//   colorTemp: {range: [153, 500]}}). tuyaLight is a standard-cluster light
//   (genOnOff 0x0006 + genLevelCtrl 0x0008 + lightingColorCtrl 0x0300 XY/HS
//   + colour-temperature) — NOT a 0xEF00 datapoint device (cf. the generic
//   tuya/TS0505B.cpp, which wires the same standard converters). This is a
//   mains-powered RGB+CCT LED strip.
//
// The auto-port was badly wrong: it wired kFzBattery + kFzOnOff and exposed
//   state/battery/voltage — a phantom battery channel on a mains light, with
//   NO brightness, NO colour, NO colour-temperature, and NO to_zigbee at
//   all. Rewrote to a full RGB+CCT light: kFzOnOff / kFzBrightness /
//   kFzColor / kFzColorTemperature (+ matching tz), the state / brightness /
//   colour / color_temp exposes, and genOnOff + genLevelCtrl + colorCtrl
//   bindings. Dropped the genPowerCfg (0x0001) battery binding.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ajax_online {
namespace {
const FzConverter* const kFz_AJ_RGBCCT_CTRL[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_AJ_RGBCCT_CTRL[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_AJ_RGBCCT_CTRL[] = { "TS0505B" };
constexpr const char* kManus_AJ_RGBCCT_CTRL[] = { "_TZ3210_hzy4rjz3" };

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
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};
}  // namespace

extern const PreparedDefinition kDef_AJ_RGBCCT_CTRL{
    .zigbee_models=kModels_AJ_RGBCCT_CTRL, .zigbee_models_count=sizeof(kModels_AJ_RGBCCT_CTRL)/sizeof(kModels_AJ_RGBCCT_CTRL[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_AJ_RGBCCT_CTRL, .manufacturer_names_count=sizeof(kManus_AJ_RGBCCT_CTRL)/sizeof(kManus_AJ_RGBCCT_CTRL[0]),
    .model="AJ_RGBCCT_CTRL", .vendor="AjaxOnline",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AJ_RGBCCT_CTRL, .from_zigbee_count=sizeof(kFz_AJ_RGBCCT_CTRL)/sizeof(kFz_AJ_RGBCCT_CTRL[0]),
    .to_zigbee=kTz_AJ_RGBCCT_CTRL, .to_zigbee_count=sizeof(kTz_AJ_RGBCCT_CTRL)/sizeof(kTz_AJ_RGBCCT_CTRL[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ajax_online
