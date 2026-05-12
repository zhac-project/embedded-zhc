// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Skydance WZ5_dim_1 — hand-rewritten for parity (BLOCKED, DP wiring).
// Zigbee & RF 5 in 1 LED controller (DIM mode), TS0601 / _TZE200_6qoazbre|fcooykb4.
// z2m: fromZigbee=[legacy.fz.tuya_light_wz5], toZigbee=[legacy.tz.tuya_dimmer_state,
//      legacy.tz.tuya_light_wz5] — Tuya 0xEF00 DP frames carry state+brightness.
// Status: BLOCKED. ZHC lacks a WZ5 TuyaDatapointMap; both decode and SET path
// are non-functional until the Tuya DP runtime gains a tuya_light_wz5 map.
// The auto-generator's genOnOff/genLevelCtrl bundle was wrong-bundle for TS0601
// (no traffic on those clusters); cleared to nullptr to make the BLOCKED state
// honest. Bindings corrected to 0xEF00 so on-join binding lands on the right
// cluster once the DP map is wired.
// z2m-source: skydance.ts #WZ5_dim_1.
#include "definitions/skydance/_shared.hpp"

namespace zhc::devices::skydance {
namespace {

constexpr const char* kModels_WZ5_dim_1[] = { "TS0601" };
constexpr const char* kManus_WZ5_dim_1[]  = { "_TZE200_6qoazbre", "_TZE200_fcooykb4" };

}  // namespace

extern const PreparedDefinition kDef_WZ5_dim_1{
    .zigbee_models=kModels_WZ5_dim_1, .zigbee_models_count=sizeof(kModels_WZ5_dim_1)/sizeof(kModels_WZ5_dim_1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_WZ5_dim_1, .manufacturer_names_count=sizeof(kManus_WZ5_dim_1)/sizeof(kManus_WZ5_dim_1[0]),
    .model="WZ5_dim_1", .vendor="Skydance",
    .meta=nullptr, .exposes=kExpWZ5Base, .exposes_count=kExpWZ5BaseCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindWZ5Tuya, .bindings_count=kBindWZ5TuyaCount,
};

}  // namespace zhc::devices::skydance
