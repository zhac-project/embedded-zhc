// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Woox R9077 — RGB+CCT LED bulb (TS0505A standard ZCL).
// z2m-source: woox.ts #R9077 (tuya.modernExtend.tuyaLight {colorTemp, color}).
//
// Standard-ZCL light: genOnOff + genLevelCtrl + lightingColorCtrl.
// No DP-stream; the wrong-bundle generator collapsed this to
// on/off + battery (mains-powered LEDs don't even have a battery).
// Replaced with the conventional 4-converter light bundle. Color
// (XY/HS) tz support follows once `kTzColor` is in the canonical
// generic kit; CT axis is wired today.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::woox {
namespace {

const FzConverter* const kFz_R9077[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_R9077[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};

constexpr const char* kModels_R9077[] = { "TS0505A" };
constexpr const char* kManus_R9077[] = { "_TZ3000_keabpigv" };

// Note: z2m exposes `color` as a composite XY/HS object; ZHC's
// ExposeType has no Composite variant yet so we expose the raw axes
// individually. Adapter consumers can recompose if needed.
constexpr Expose kExp_R9077[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired",  nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_R9077[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl
};

}  // namespace

extern const PreparedDefinition kDef_R9077{
    .zigbee_models=kModels_R9077, .zigbee_models_count=sizeof(kModels_R9077)/sizeof(kModels_R9077[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_R9077, .manufacturer_names_count=sizeof(kManus_R9077)/sizeof(kManus_R9077[0]),
    .model="R9077", .vendor="Woox",
    .meta=nullptr, .exposes=kExp_R9077, .exposes_count=sizeof(kExp_R9077)/sizeof(kExp_R9077[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_R9077, .from_zigbee_count=sizeof(kFz_R9077)/sizeof(kFz_R9077[0]),
    .to_zigbee=kTz_R9077,   .to_zigbee_count=sizeof(kTz_R9077)/sizeof(kTz_R9077[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_R9077,
    .bindings_count=sizeof(kBind_R9077)/sizeof(kBind_R9077[0]),
};

}  // namespace zhc::devices::woox
