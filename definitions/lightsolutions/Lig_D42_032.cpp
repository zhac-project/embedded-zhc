// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lightsolutions 42-032 — CCT LED driver.
// LED driver CCT 12V - 30W - CCT
// z2m-source: lightsolutions.ts #42-032 — m.light({colorTemp:{range:[160,450]}}).
//
// The auto-port dropped the colorTemp axis: z2m wires a CCT light
// (lightingColorCtrl 0x0300 colorTemperature) but the generated def only
// carried OnOff + Brightness. Add the generic colorTemp fz/tz, a `color_temp`
// expose and the 0x0300 binding so tunable-white round-trips.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lightsolutions {
namespace {
const FzConverter* const kFz_D42_032[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_D42_032[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_D42_032[] = { "42-032" };

}  // namespace


// --- Tier 2 override: CCT light (state + brightness + color_temp) ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};
// --- end Tier 2 override block ---

extern const PreparedDefinition kDef_D42_032{
    .zigbee_models=kModels_D42_032, .zigbee_models_count=sizeof(kModels_D42_032)/sizeof(kModels_D42_032[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="42-032", .vendor="Lightsolutions",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D42_032, .from_zigbee_count=sizeof(kFz_D42_032)/sizeof(kFz_D42_032[0]),
    .to_zigbee=kTz_D42_032, .to_zigbee_count=sizeof(kTz_D42_032)/sizeof(kTz_D42_032[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::lightsolutions
