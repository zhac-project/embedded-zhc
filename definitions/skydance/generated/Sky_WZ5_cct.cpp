// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Skydance WZ5_cct — hand-rewritten for parity (BLOCKED, DP wiring).
// Zigbee & RF 5 in 1 LED controller (CCT mode), TS0601 / _TZE200_gz3n0tzf|nthosjmx|na98lvjp|2gtsuokt.
// z2m: legacy.fz.tuya_light_wz5 / [legacy.tz.tuya_dimmer_state, legacy.tz.tuya_light_wz5];
//      exposes state+brightness + color_temp[250..454]. Whitelabels:
//      Ltech TY-75-24-G2Z2_CCT, Ltech SE-20-250-1000-W2Z2.
// Status: BLOCKED — see _shared.hpp comment.
// z2m-source: skydance.ts #WZ5_cct.
#include "definitions/skydance/_shared.hpp"

namespace zhc::devices::skydance {
namespace {

constexpr const char* kModels_WZ5_cct[] = { "TS0601" };
constexpr const char* kManus_WZ5_cct[]  = {
    "_TZE200_gz3n0tzf", "_TZE200_nthosjmx", "_TZE200_na98lvjp", "_TZE200_2gtsuokt",
};

constexpr Expose kExp_WZ5_cct[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired",  nullptr, nullptr, 0},
};

constexpr WhiteLabel kWL_WZ5_cct[] = {
    {"Ltech", "TY-75-24-G2Z2_CCT"},
    {"Ltech", "SE-20-250-1000-W2Z2"},
};

}  // namespace

extern const PreparedDefinition kDef_WZ5_cct{
    .zigbee_models=kModels_WZ5_cct, .zigbee_models_count=sizeof(kModels_WZ5_cct)/sizeof(kModels_WZ5_cct[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_WZ5_cct, .manufacturer_names_count=sizeof(kManus_WZ5_cct)/sizeof(kManus_WZ5_cct[0]),
    .model="WZ5_cct", .vendor="Skydance",
    .meta=nullptr, .exposes=kExp_WZ5_cct, .exposes_count=sizeof(kExp_WZ5_cct)/sizeof(kExp_WZ5_cct[0]),
    .white_labels=kWL_WZ5_cct, .white_labels_count=sizeof(kWL_WZ5_cct)/sizeof(kWL_WZ5_cct[0]),
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindWZ5Tuya, .bindings_count=kBindWZ5TuyaCount,
};

}  // namespace zhc::devices::skydance
