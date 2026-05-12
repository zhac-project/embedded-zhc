// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Mercator SMD4109W-RGB-ZB — hand-rewritten (was wrong-bundle).
// 92mm Walter downlight RGB + CCT.
//
// z2m extend: tuya.modernExtend.tuyaLight({colorTemp:{range:[153,500]}, color:true})
//   → on/off + brightness + colorTemp + color (xy + hs)  →  ColorCTLight bundle.
//
// The auto-generator collapsed this to on/off + battery (mis-typed)
// and dropped both the colorTemp and color axes. Fixed in 2026-04-28p
// MERCATOR sweep — only one Mercator light SKU, so the bundle is
// inlined here instead of a shared file.
//
// z2m-source: mercator.ts #SMD4109W-RGB-ZB.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::mercator {
namespace {

const FzConverter* const kFz_SMD4109W_RGB_ZB[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_SMD4109W_RGB_ZB[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};

constexpr const char* kModels_SMD4109W_RGB_ZB[] = { "TS0505B" };
constexpr const char* kManus_SMD4109W_RGB_ZB[] = { "_TZ3000_xr5m6kfg" };

constexpr Expose kExposes_SMD4109W_RGB_ZB[] = {
    { "state",      ExposeType::Binary,  Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "brightness", ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "color_temp", ExposeType::Numeric, Access::StateSet, "mired",  nullptr, nullptr, 0 },
    { "color_x",    ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "color_y",    ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "hue",        ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "saturation", ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_SMD4109W_RGB_ZB[] = {
    { 1, 0x0006 },  // genOnOff
    { 1, 0x0008 },  // genLevelCtrl
    { 1, 0x0300 },  // lightingColorCtrl
};

}  // namespace

extern const PreparedDefinition kDef_SMD4109W_RGB_ZB{
    .zigbee_models=kModels_SMD4109W_RGB_ZB, .zigbee_models_count=sizeof(kModels_SMD4109W_RGB_ZB)/sizeof(kModels_SMD4109W_RGB_ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SMD4109W_RGB_ZB, .manufacturer_names_count=sizeof(kManus_SMD4109W_RGB_ZB)/sizeof(kManus_SMD4109W_RGB_ZB[0]),
    .model="SMD4109W-RGB-ZB", .vendor="Mercator",
    .meta=nullptr,
    .exposes=kExposes_SMD4109W_RGB_ZB, .exposes_count=sizeof(kExposes_SMD4109W_RGB_ZB)/sizeof(kExposes_SMD4109W_RGB_ZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SMD4109W_RGB_ZB, .from_zigbee_count=sizeof(kFz_SMD4109W_RGB_ZB)/sizeof(kFz_SMD4109W_RGB_ZB[0]),
    .to_zigbee=kTz_SMD4109W_RGB_ZB, .to_zigbee_count=sizeof(kTz_SMD4109W_RGB_ZB)/sizeof(kTz_SMD4109W_RGB_ZB[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SMD4109W_RGB_ZB, .bindings_count=sizeof(kBindings_SMD4109W_RGB_ZB)/sizeof(kBindings_SMD4109W_RGB_ZB[0]),
};

}  // namespace zhc::devices::mercator
