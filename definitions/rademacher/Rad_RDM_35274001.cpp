// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Rademacher RDM-35274001 — graduated from generated/ for a parity fix.
// addZ white + colour E27 LED
// z2m extend: m.light({colorTemp: {range: [153, 555]},
//                      color: {modes: ["xy", "hs"], enhancedHue: true}})
//   → on/off + level + colorTemp + color (xy + hs).
// The auto-generator collapsed m.light() to on/off + brightness only,
// dropping the colour-temperature and colour axes — this file restores them
// (kFzColorTemperature emits color_temp; kFzColor emits color_x/color_y/hue/
// saturation) and adds the lightingColorCtrl (0x0300) binding.
// z2m-source: rademacher.ts #RDM-35274001.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::rademacher {
namespace {
const FzConverter* const kFz_RDM_35274001[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_RDM_35274001[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_RDM_35274001[] = { "RDM-35274001" };

}  // namespace


// --- hand-curated ColorCTLight surface (was on/off + brightness only) ---
constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired",  nullptr, nullptr, 0},
    {"color_x",    ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"color_y",    ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"hue",        ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (CT + color)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_RDM_35274001{
    .zigbee_models=kModels_RDM_35274001, .zigbee_models_count=sizeof(kModels_RDM_35274001)/sizeof(kModels_RDM_35274001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RDM-35274001", .vendor="Rademacher",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_RDM_35274001, .from_zigbee_count=sizeof(kFz_RDM_35274001)/sizeof(kFz_RDM_35274001[0]),
    .to_zigbee=kTz_RDM_35274001, .to_zigbee_count=sizeof(kTz_RDM_35274001)/sizeof(kTz_RDM_35274001[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::rademacher
