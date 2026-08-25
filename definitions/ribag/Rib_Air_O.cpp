// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: RIBAG Licht "Ribag Air O" Vertico Air pendant light (CT).
// New vendor upstream in z2m v26.98.0.
//
// z2m: `m.light({colorTemp: {range: [153, 500]}})` plus
// `meta: {omitOptionalLevelAndColorParams: true}` — the lamp rejects
// move-to-level / move-to-color-temp commands that carry the optional
// transition and options fields, so those params must be omitted from
// the outbound frame. ZHC's generic level/colour-temp tz emit the short
// form already, so the meta flag needs no counterpart here; it is
// recorded in this comment so a future long-form tz does not silently
// break the lamp.
//
// z2m-source: ribag.ts #Ribag Air O.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ribag {
namespace {

const FzConverter* const kFz_AirO[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_AirO[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};

constexpr const char* kModels_AirO[] = { "Ribag Air O" };

constexpr Expose kExposes_AirO[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0,
     ExposeCategory::State, 153, 500, 1},
};

constexpr BindingSpec kBindings_AirO[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0300},   // lightingColorCtrl
};

}  // namespace

extern const PreparedDefinition kDef_AirO{
    .zigbee_models=kModels_AirO, .zigbee_models_count=sizeof(kModels_AirO)/sizeof(kModels_AirO[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Ribag Air O", .vendor="RIBAG Licht",
    .meta=nullptr,
    .exposes=kExposes_AirO, .exposes_count=sizeof(kExposes_AirO)/sizeof(kExposes_AirO[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AirO, .from_zigbee_count=sizeof(kFz_AirO)/sizeof(kFz_AirO[0]),
    .to_zigbee=kTz_AirO, .to_zigbee_count=sizeof(kTz_AirO)/sizeof(kTz_AirO[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_AirO, .bindings_count=sizeof(kBindings_AirO)/sizeof(kBindings_AirO[0]),
};

}  // namespace zhc::devices::ribag
