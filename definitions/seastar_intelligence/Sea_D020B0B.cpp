// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: SeastarIntelligence 020B0B — graduated override.
// LED Tischleuchte Beta Zig (Fischer & Honsel) — RGB + tunable-white bulb.
//
// z2m: m.light({colorTemp:{range:[153,370]}, color:true}) → on/off +
// brightness + colorTemp (mireds) + color (xy + hs). The Tier-1 auto-port
// collapsed it to on/off + brightness only, dropping the entire 0x0300
// lightingColorCtrl color/colorTemp axis. Restored kFzColorTemperature +
// kFzColor (+ kTzColorTemp / kTzColor), the color_temp / color_x / color_y /
// hue / saturation exposes, and the 0x0300 bind.
//
// z2m-source: seastar_intelligence.ts #020B0B.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::seastar_intelligence {
namespace {
const FzConverter* const kFz_D020B0B[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_D020B0B[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_D020B0B[] = { "020B0B" };

}  // namespace


// --- restored color/colorTemp axis (z2m m.light(colorTemp+color:true)) ---
constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_x",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};
// --- end restored block ---

extern const PreparedDefinition kDef_D020B0B{
    .zigbee_models=kModels_D020B0B, .zigbee_models_count=sizeof(kModels_D020B0B)/sizeof(kModels_D020B0B[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="020B0B", .vendor="SeastarIntelligence",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D020B0B, .from_zigbee_count=sizeof(kFz_D020B0B)/sizeof(kFz_D020B0B[0]),
    .to_zigbee=kTz_D020B0B, .to_zigbee_count=sizeof(kTz_D020B0B)/sizeof(kTz_D020B0B[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::seastar_intelligence
