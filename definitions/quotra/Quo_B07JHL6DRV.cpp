// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Quotra B07JHL6DRV — graduated from generated/ to restore the
//   colour + colour-temperature axes that the auto-port dropped.
// RGB WW LED strip
// z2m-source: quotra.ts #B07JHL6DRV —
//   m.light({colorTemp:{range:[150,500]}, color:true, powerOnBehavior:false})
//
// Bug: the auto-generated def reduced this RGB+CCT strip to a plain
//   on/off + brightness dimmer (exposes state/brightness only; fz/tz =
//   kFzOnOff+kFzBrightness). z2m's m.light with color:true and a colorTemp
//   range exposes the full colour surface (lightingColorCtrl 0x0300:
//   color_temp + colour xy). Restored here by wiring the generic colour
//   converters, the color_temp/color_xy exposes, and the 0x0300 bind —
//   mirroring the Lonsonho ZB-RGBCW RGBCW analog.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::quotra {
namespace {
const FzConverter* const kFz_B07JHL6DRV[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_B07JHL6DRV[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_B07JHL6DRV[] = { "QV-RGBCCT" };

}  // namespace


// --- colour surface restored (was state/brightness only) ---
// z2m: m.light({colorTemp:{range:[150,500]}, color:true}) — RGB+WW strip
constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_xy",   ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};
// --- end colour surface block ---

extern const PreparedDefinition kDef_B07JHL6DRV{
    .zigbee_models=kModels_B07JHL6DRV, .zigbee_models_count=sizeof(kModels_B07JHL6DRV)/sizeof(kModels_B07JHL6DRV[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="B07JHL6DRV", .vendor="Quotra",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_B07JHL6DRV, .from_zigbee_count=sizeof(kFz_B07JHL6DRV)/sizeof(kFz_B07JHL6DRV[0]),
    .to_zigbee=kTz_B07JHL6DRV, .to_zigbee_count=sizeof(kTz_B07JHL6DRV)/sizeof(kTz_B07JHL6DRV[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::quotra
