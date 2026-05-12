// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Feibit FZCWD2HW-YH — hand-rewritten ColorCTLight bundle.
// Smart LED Recessed Light.
// z2m extend: m.identify(), m.light({colorTemp: {range: [153, 370]},
//                                    color: {modes: ["xy", "hs"], enhancedHue: true}})
//   → on/off + level + colorTemp + color (xy + hs).
// The auto-generator collapses m.light() to on/off + brightness only,
// dropping the CT/color axes — this file restores them.
// z2m-source: feibit.ts #FZCWD2HW-YH.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::feibit {
namespace {
const FzConverter* const kFz_FZCWD2HW_YH[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_FZCWD2HW_YH[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_FZCWD2HW_YH[] = { "FZT56-ZCW2HYH1.3" };

}  // namespace


// Hand-curated ColorCTLight surface (m.identify() adds 0x0003 binding).
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
    {1, 0x0003},  // genIdentify (m.identify())
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (CT + color)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_FZCWD2HW_YH{
    .zigbee_models=kModels_FZCWD2HW_YH, .zigbee_models_count=sizeof(kModels_FZCWD2HW_YH)/sizeof(kModels_FZCWD2HW_YH[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="FZCWD2HW-YH", .vendor="Feibit",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_FZCWD2HW_YH, .from_zigbee_count=sizeof(kFz_FZCWD2HW_YH)/sizeof(kFz_FZCWD2HW_YH[0]),
    .to_zigbee=kTz_FZCWD2HW_YH, .to_zigbee_count=sizeof(kTz_FZCWD2HW_YH)/sizeof(kTz_FZCWD2HW_YH[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::feibit
