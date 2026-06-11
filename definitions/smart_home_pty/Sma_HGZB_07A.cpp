// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: SmartHomePty HGZB-07A — graduated from generated/ for a parity fix.
// RGBW Downlight
// z2m extend: m.light({colorTemp: {range: undefined}, color: true})
//   → on/off + level + colorTemp + color (xy).
//
// Parity fix: the auto-generator collapses m.light() to on/off + brightness
// only, dropping the color-temperature and colour (xy) axes. This RGBW
// downlight is exactly that case — restore the CT/colour channels via the
// generic lightingColorCtrl (0x0300) converters:
//   * kFzColorTemperature / kTzColorTemp  → color_temp (mireds, attr 0x0007)
//   * kFzColor            / kTzColor       → color_x/color_y (attr 0x0003/0x0004)
// z2m's `color: true` is xy-mode by default; the generic kFzColor decodes the
// full ColorControl surface (hue/sat/xy/color_mode) in one pass, so hue/
// saturation exposes are included for completeness (the same surface the
// Feibit FZCWF2LW-BW parity fix uses).
// z2m-source: smart_home_pty.ts #HGZB-07A.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smart_home_pty {
namespace {
const FzConverter* const kFz_HGZB_07A[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_HGZB_07A[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_HGZB_07A[] = { "FB56-ZCW11HG1.2", "FB56-ZCW11HG1.4" };

}  // namespace


// Hand-curated ColorCTLight surface (restores the CT + colour axes the
// auto-generator dropped from m.light({colorTemp, color: true})).
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
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (CT + colour)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_HGZB_07A{
    .zigbee_models=kModels_HGZB_07A, .zigbee_models_count=sizeof(kModels_HGZB_07A)/sizeof(kModels_HGZB_07A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HGZB-07A", .vendor="SmartHomePty",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HGZB_07A, .from_zigbee_count=sizeof(kFz_HGZB_07A)/sizeof(kFz_HGZB_07A[0]),
    .to_zigbee=kTz_HGZB_07A, .to_zigbee_count=sizeof(kTz_HGZB_07A)/sizeof(kTz_HGZB_07A[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::smart_home_pty
