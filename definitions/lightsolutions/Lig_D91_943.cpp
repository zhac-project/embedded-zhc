// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lightsolutions 91-943 — RGBW + CCT LED controller.
// LED Controller 12/24v
// z2m-source: lightsolutions.ts #91-943 —
//   m.light({color: true, colorTemp: {range: [160, 450]}}).
//
// The auto-port dropped BOTH the color (lightingColorCtrl 0x0300 hue/sat/xy)
// and colorTemp axes, leaving a bare OnOff + Brightness dimmer. z2m exposes a
// full RGBW + tunable-white bulb. Add the generic color + colorTemp fz/tz,
// `color_temp` + `color_xy` exposes and the 0x0300 binding.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lightsolutions {
namespace {
const FzConverter* const kFz_D91_943[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_D91_943[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_D91_943[] = { "91-943", "01F" };

}  // namespace


// --- Tier 2 override: RGBW+CCT light (state + brightness + color_temp + color_xy) ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_xy", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};
// --- end Tier 2 override block ---

extern const PreparedDefinition kDef_D91_943{
    .zigbee_models=kModels_D91_943, .zigbee_models_count=sizeof(kModels_D91_943)/sizeof(kModels_D91_943[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="91-943", .vendor="Lightsolutions",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D91_943, .from_zigbee_count=sizeof(kFz_D91_943)/sizeof(kFz_D91_943[0]),
    .to_zigbee=kTz_D91_943, .to_zigbee_count=sizeof(kTz_D91_943)/sizeof(kTz_D91_943[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::lightsolutions
