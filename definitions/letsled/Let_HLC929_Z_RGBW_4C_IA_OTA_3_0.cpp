// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Letsled HLC929-Z-RGBW-4C-IA-OTA-3.0 — hand-rewritten ColorCTLight bundle.
// RGBW down light (color temp is inverted).
// z2m extend: m.light({colorTemp: {range: [153, 370]}, color: true})
//   → on/off + level + colorTemp + color (xy + hs).
// The auto-generator collapsed m.light() to on/off + brightness only,
// dropping the CT/color axes — this file restores them.
// z2m-source: letsled.ts #HLC929-Z-RGBW-4C-IA-OTA-3.0.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::letsled {
namespace {
const FzConverter* const kFz_HLC929_Z_RGBW_4C_IA_OTA_3_0[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_HLC929_Z_RGBW_4C_IA_OTA_3_0[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_HLC929_Z_RGBW_4C_IA_OTA_3_0[] = { "RGBW Down Light" };

}  // namespace


// Hand-curated ColorCTLight surface (colorTemp range 153-370 mireds, color xy+hs).
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

extern const PreparedDefinition kDef_HLC929_Z_RGBW_4C_IA_OTA_3_0{
    .zigbee_models=kModels_HLC929_Z_RGBW_4C_IA_OTA_3_0, .zigbee_models_count=sizeof(kModels_HLC929_Z_RGBW_4C_IA_OTA_3_0)/sizeof(kModels_HLC929_Z_RGBW_4C_IA_OTA_3_0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HLC929-Z-RGBW-4C-IA-OTA-3.0", .vendor="Letsled",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HLC929_Z_RGBW_4C_IA_OTA_3_0, .from_zigbee_count=sizeof(kFz_HLC929_Z_RGBW_4C_IA_OTA_3_0)/sizeof(kFz_HLC929_Z_RGBW_4C_IA_OTA_3_0[0]),
    .to_zigbee=kTz_HLC929_Z_RGBW_4C_IA_OTA_3_0, .to_zigbee_count=sizeof(kTz_HLC929_Z_RGBW_4C_IA_OTA_3_0)/sizeof(kTz_HLC929_Z_RGBW_4C_IA_OTA_3_0[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::letsled
