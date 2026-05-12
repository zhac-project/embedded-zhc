// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Icasa ICZB-FC filament lamp — hand-rewritten CT light bundle.
// z2m: m.light({colorTemp: {range: undefined}}) — on/off + brightness +
// colorTemp (mireds). Auto-generator collapsed this to on/off + brightness
// only; rewrite restores the colorTemp axis.
// z2m-source: icasa.ts #ICZB-B1FC60/B3FC64/B2FC95/B2FC125.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::icasa {
namespace {
const FzConverter* const kFz_ICZB_B1FC60_B3FC64_B2FC95_B2FC125[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_ICZB_B1FC60_B3FC64_B2FC95_B2FC125[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_ICZB_B1FC60_B3FC64_B2FC95_B2FC125[] = { "ICZB-FC" };

constexpr Expose kExposes_ICZB_B1FC60_B3FC64_B2FC95_B2FC125[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ICZB_B1FC60_B3FC64_B2FC95_B2FC125[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl
};

}  // namespace

extern const PreparedDefinition kDef_ICZB_B1FC60_B3FC64_B2FC95_B2FC125{
    .zigbee_models=kModels_ICZB_B1FC60_B3FC64_B2FC95_B2FC125,
    .zigbee_models_count=sizeof(kModels_ICZB_B1FC60_B3FC64_B2FC95_B2FC125)/sizeof(kModels_ICZB_B1FC60_B3FC64_B2FC95_B2FC125[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ICZB-B1FC60/B3FC64/B2FC95/B2FC125", .vendor="Icasa",
    .meta=nullptr,
    .exposes=kExposes_ICZB_B1FC60_B3FC64_B2FC95_B2FC125,
    .exposes_count=sizeof(kExposes_ICZB_B1FC60_B3FC64_B2FC95_B2FC125)/sizeof(kExposes_ICZB_B1FC60_B3FC64_B2FC95_B2FC125[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ICZB_B1FC60_B3FC64_B2FC95_B2FC125,
    .from_zigbee_count=sizeof(kFz_ICZB_B1FC60_B3FC64_B2FC95_B2FC125)/sizeof(kFz_ICZB_B1FC60_B3FC64_B2FC95_B2FC125[0]),
    .to_zigbee=kTz_ICZB_B1FC60_B3FC64_B2FC95_B2FC125,
    .to_zigbee_count=sizeof(kTz_ICZB_B1FC60_B3FC64_B2FC95_B2FC125)/sizeof(kTz_ICZB_B1FC60_B3FC64_B2FC95_B2FC125[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ICZB_B1FC60_B3FC64_B2FC95_B2FC125,
    .bindings_count=sizeof(kBindings_ICZB_B1FC60_B3FC64_B2FC95_B2FC125)/sizeof(kBindings_ICZB_B1FC60_B3FC64_B2FC95_B2FC125[0]),
};

}  // namespace zhc::devices::icasa
