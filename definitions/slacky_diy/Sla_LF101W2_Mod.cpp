// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Svetomaniya "Ledron LF101W2" RGBW light with the Slacky-DiY
// firmware (`LF101W2-Mod`). New upstream in z2m v26.101.0.
//
// A full colour light: on/off, brightness, xy colour and a NARROW colour
// temperature range of 454-500 mired. That range is unusually tight — the
// lamp's white channel barely moves — so it is declared explicitly rather
// than left to the generic default, which would show a slider spanning a
// range the lamp cannot reach.
//
// `turnsOffAtBrightness1` upstream flags that brightness 1 switches the lamp
// off instead of dimming; that is a host-side workaround in z2m's command
// path and has no decode-side counterpart here.
//
// DEFERRED: `min_level` / `max_level`, custom attributes 0xF002 / 0xF003 the
// firmware adds to genLevelCtrl, plus the levelConfig transition-time set.
// Both need manufacturer attribute writes against a custom cluster with no
// generic tz to reuse.
//
// z2m-source: slacky_diy.ts #LF101W2-Mod.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::slacky_diy {
namespace {

const FzConverter* const kFz_LF101W2_Mod[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_LF101W2_Mod[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};

constexpr const char* kModels_LF101W2_Mod[] = { "LF101W2-Mod" };

constexpr Expose kExposes_LF101W2_Mod[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0,
     ExposeCategory::State, 454, 500, 1},
    {"color",      ExposeType::String,  Access::StateSet, nullptr, "xy colour", nullptr, 0},
};

constexpr BindingSpec kBindings_LF101W2_Mod[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0300},   // lightingColorCtrl
};

}  // namespace

extern const PreparedDefinition kDef_LF101W2_Mod{
    .zigbee_models=kModels_LF101W2_Mod,
    .zigbee_models_count=sizeof(kModels_LF101W2_Mod)/sizeof(kModels_LF101W2_Mod[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LF101W2-Mod", .vendor="Svetomaniya",
    .meta=nullptr,
    .exposes=kExposes_LF101W2_Mod,
    .exposes_count=sizeof(kExposes_LF101W2_Mod)/sizeof(kExposes_LF101W2_Mod[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_LF101W2_Mod, .from_zigbee_count=sizeof(kFz_LF101W2_Mod)/sizeof(kFz_LF101W2_Mod[0]),
    .to_zigbee=kTz_LF101W2_Mod, .to_zigbee_count=sizeof(kTz_LF101W2_Mod)/sizeof(kTz_LF101W2_Mod[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_LF101W2_Mod,
    .bindings_count=sizeof(kBindings_LF101W2_Mod)/sizeof(kBindings_LF101W2_Mod[0]),
};

}  // namespace zhc::devices::slacky_diy
