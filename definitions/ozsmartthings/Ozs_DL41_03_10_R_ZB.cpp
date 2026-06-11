// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ozsmartthings DL41-03-10-R-ZB — hand-rewritten RGBW light bundle.
// Oz Smart RGBW Zigbee downlight 10w. z2m extend =
// tuya.modernExtend.tuyaLight({colorTemp:{range:[153,500]}, color:true}) →
// modernExtend.light() with the colorTemp + color axes → on/off + brightness
// + colorTemp (mireds) + color (xy + hs). The auto-port collapsed this to a
// battery + on/off sensor stub (phantom battery/voltage exposes, genPowerCfg
// bind), dropping the entire light. This rewrite restores the real light
// class: kFzOnOff + kFzBrightness + kFzColorTemperature + kFzColor
// (+ kTzOnOff/kTzBrightness/kTzColorTemp/kTzColor), bind 0x0006/0x0008/0x0300.
// (do_not_disturb + color_power_on_behavior tuya toggles are write-only DP/
// attribute UI extras with no generic converter — INFRA, deferred.)
// z2m-source: ozsmartthings.ts #DL41-03-10-R-ZB.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ozsmartthings {
namespace {

const FzConverter* const kFz_DL41_03_10_R_ZB[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_DL41_03_10_R_ZB[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};

constexpr const char* kModels_DL41_03_10_R_ZB[] = { "TS0505B" };
constexpr const char* kManus_DL41_03_10_R_ZB[] = { "_TZ3210_mcm6m1ma", "_TZ3210_klsm24op", "_TZ3210_pdqu9pot" };

constexpr Expose kExposes_DL41_03_10_R_ZB[] = {
    {"state",      ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_x",    ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y",    ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue",        ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_DL41_03_10_R_ZB[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (CT + color)
};

}  // namespace

extern const PreparedDefinition kDef_DL41_03_10_R_ZB{
    .zigbee_models=kModels_DL41_03_10_R_ZB, .zigbee_models_count=sizeof(kModels_DL41_03_10_R_ZB)/sizeof(kModels_DL41_03_10_R_ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_DL41_03_10_R_ZB, .manufacturer_names_count=sizeof(kManus_DL41_03_10_R_ZB)/sizeof(kManus_DL41_03_10_R_ZB[0]),
    .model="DL41-03-10-R-ZB", .vendor="Ozsmartthings",
    .meta=nullptr, .exposes=kExposes_DL41_03_10_R_ZB, .exposes_count=sizeof(kExposes_DL41_03_10_R_ZB)/sizeof(kExposes_DL41_03_10_R_ZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DL41_03_10_R_ZB, .from_zigbee_count=sizeof(kFz_DL41_03_10_R_ZB)/sizeof(kFz_DL41_03_10_R_ZB[0]),
    .to_zigbee=kTz_DL41_03_10_R_ZB, .to_zigbee_count=sizeof(kTz_DL41_03_10_R_ZB)/sizeof(kTz_DL41_03_10_R_ZB[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_DL41_03_10_R_ZB,
    .bindings_count           = sizeof(kBind_DL41_03_10_R_ZB)/sizeof(kBind_DL41_03_10_R_ZB[0]),
};

}  // namespace zhc::devices::ozsmartthings
