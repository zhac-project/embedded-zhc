// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Skydance WZ5_rgb — hand-rewritten for parity (BLOCKED, DP wiring).
// Zigbee & RF 5 in 1 LED controller (RGB mode), TS0601 / _TZE200_9hghastn|9mt3kgn0.
// z2m: legacy.fz.tuya_light_wz5 / [legacy.tz.tuya_dimmer_state, legacy.tz.tuya_light_wz5];
//      exposes state+brightness + color_hs.
// Status: BLOCKED — see _shared.hpp comment.
// z2m-source: skydance.ts #WZ5_rgb.
#include "definitions/skydance/_shared.hpp"

namespace zhc::devices::skydance {
namespace {

constexpr const char* kModels_WZ5_rgb[] = { "TS0601" };
constexpr const char* kManus_WZ5_rgb[]  = { "_TZE200_9hghastn", "_TZE200_9mt3kgn0" };

constexpr Expose kExp_WZ5_rgb[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_hs",   ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

}  // namespace

extern const PreparedDefinition kDef_WZ5_rgb{
    .zigbee_models=kModels_WZ5_rgb, .zigbee_models_count=sizeof(kModels_WZ5_rgb)/sizeof(kModels_WZ5_rgb[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_WZ5_rgb, .manufacturer_names_count=sizeof(kManus_WZ5_rgb)/sizeof(kManus_WZ5_rgb[0]),
    .model="WZ5_rgb", .vendor="Skydance",
    .meta=nullptr, .exposes=kExp_WZ5_rgb, .exposes_count=sizeof(kExp_WZ5_rgb)/sizeof(kExp_WZ5_rgb[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindWZ5Tuya, .bindings_count=kBindWZ5TuyaCount,
};

}  // namespace zhc::devices::skydance
