// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Sowilo L258 — graduated override (full colour surface restored).
// Heimdall Pro 5 channel RGBWW controller.
//
// Bug fixed: z2m drives this as
//   m.light({colorTemp:{range:[150,575]}, color:{modes:["xy","hs"]},
//            turnsOffAtBrightness1:true})
// i.e. a full RGB + tunable-white controller. The auto-generated Tier-1
// port dropped BOTH colour axes, lowering it to on/off + brightness only.
// Restore the colorTemp + colour decoders/setters, the color_temp /
// color_xy exposes, and the lightingColorCtrl (0x0300) binding so the
// RGBWW surface round-trips.
// z2m-source: sowilo.ts #L258.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sowilo {
namespace {
const FzConverter* const kFz_L258[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_L258[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_L258[] = { "L258" };

}  // namespace


// --- colour-light surface (mirrors z2m m.light colorTemp + color) ---
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
// --- end colour-light surface ---

extern const PreparedDefinition kDef_L258{
    .zigbee_models=kModels_L258, .zigbee_models_count=sizeof(kModels_L258)/sizeof(kModels_L258[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="L258", .vendor="Sowilo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_L258, .from_zigbee_count=sizeof(kFz_L258)/sizeof(kFz_L258[0]),
    .to_zigbee=kTz_L258, .to_zigbee_count=sizeof(kTz_L258)/sizeof(kTz_L258[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::sowilo
