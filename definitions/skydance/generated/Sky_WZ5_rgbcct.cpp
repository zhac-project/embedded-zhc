// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Skydance WZ5_rgbcct — hand-rewritten for parity (BLOCKED, DP wiring).
// Zigbee & RF 5 in 1 LED controller (RGB+CCT mode), TS0601 / _TZE200_mde0utnv|aa9awrng.
// z2m: legacy.fz.tuya_light_wz5 / [legacy.tz.tuya_dimmer_state, legacy.tz.tuya_light_wz5];
//      exposes state+brightness + color_temp[250..454] + color_hs + numeric
//      white_brightness[0..254] (meta:{separateWhite:true}).
// Status: BLOCKED — see _shared.hpp comment.
// z2m-source: skydance.ts #WZ5_rgbcct.
#include "definitions/skydance/_shared.hpp"

namespace zhc::devices::skydance {
namespace {

constexpr const char* kModels_WZ5_rgbcct[] = { "TS0601" };
constexpr const char* kManus_WZ5_rgbcct[]  = { "_TZE200_mde0utnv", "_TZE200_aa9awrng" };

constexpr Expose kExp_WZ5_rgbcct[] = {
    {"state",            ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness",       ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp",       ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_hs",         ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"white_brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

}  // namespace

extern const PreparedDefinition kDef_WZ5_rgbcct{
    .zigbee_models=kModels_WZ5_rgbcct, .zigbee_models_count=sizeof(kModels_WZ5_rgbcct)/sizeof(kModels_WZ5_rgbcct[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_WZ5_rgbcct, .manufacturer_names_count=sizeof(kManus_WZ5_rgbcct)/sizeof(kManus_WZ5_rgbcct[0]),
    .model="WZ5_rgbcct", .vendor="Skydance",
    .meta=nullptr, .exposes=kExp_WZ5_rgbcct, .exposes_count=sizeof(kExp_WZ5_rgbcct)/sizeof(kExp_WZ5_rgbcct[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindWZ5Tuya, .bindings_count=kBindWZ5TuyaCount,
};

}  // namespace zhc::devices::skydance
