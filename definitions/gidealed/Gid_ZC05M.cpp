// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Gidealed ZC05M — graduated from generated/ to restore the
// dropped color / colorTemp axes.
//
// Smart Zigbee RGB LED strip controller. z2m models this as
//   m.light({colorTemp: {range: [153, 370]}, color: {modes: ["xy","hs"]}})
// i.e. a full RGB+CCT light: on/off + brightness + colorTemp (mireds) +
// color (xy/hs). The auto-port dropped BOTH the colorTemp and color axes,
// wiring only on/off + brightness and binding only 0x0006 + 0x0008 — so
// every color / color_temp report was silently dropped and the device
// could not be tuned. Restored kFzColorTemperature + kFzColor (and the
// matching tz writers), the color_temp / color_xy exposes, and the
// lightingColorCtrl (0x0300) binding to match z2m exactly.
//
// z2m-source: gidealed.ts #ZC05M.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::gidealed {
namespace {
const FzConverter* const kFz_ZC05M[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_ZC05M[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_ZC05M[] = { "A11" };

}  // namespace


// --- expose + binding block (hand-tuned to match z2m m.light) ---
// z2m: m.light({colorTemp:{range:[153,370]}, color:{modes:["xy","hs"]}})
constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0,
        ExposeCategory::State, 153, 370, 0},
    {"color_xy",   ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},  // lightingColorCtrl (colorTemp + color xy/hs)
};
// --- end expose + binding block ---

extern const PreparedDefinition kDef_ZC05M{
    .zigbee_models=kModels_ZC05M, .zigbee_models_count=sizeof(kModels_ZC05M)/sizeof(kModels_ZC05M[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZC05M", .vendor="Gidealed",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZC05M, .from_zigbee_count=sizeof(kFz_ZC05M)/sizeof(kFz_ZC05M[0]),
    .to_zigbee=kTz_ZC05M, .to_zigbee_count=sizeof(kTz_ZC05M)/sizeof(kTz_ZC05M[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::gidealed
