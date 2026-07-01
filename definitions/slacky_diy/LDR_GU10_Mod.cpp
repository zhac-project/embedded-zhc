// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Svetomaniya LDR-GU10-Mod — Ledron LDR-GU10 CCT bulb (new firmware),
// zigbeeModel "LDR-GU10-Mod".  (Shipped from slacky_diy.ts; z2m vendor
// "Svetomaniya".)
//
// z2m-source: slacky_diy.ts #LDR-GU10-Mod.  z2m bundles
//   m.light({colorTemp:{range:[153,500]}, turnsOffAtBrightness1:true,
//            levelConfig:{...}, configureReporting:true})
// on the standard genOnOff (0x0006) / genLevelCtrl (0x0008) /
// lightingColorCtrl (0x0300) clusters — a tunable-white (CCT) bulb:
// state + brightness + color_temp (mireds 153-500).  It additionally exposes
// two custom-attribute config controls (min_level/max_level on custom
// genLevelCtrl attrs, and a startUpColorTemperature attr) for which the ZHC
// runtime has no generic decoder; those are deferred (config-only), matching
// the envilar CCT-driver parity floor.
//
// The generic kFzOnOff/kFzBrightness/kFzColorTemperature +
// kTzOnOff/kTzBrightness/kTzColorTemp cover the full light surface.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::slacky_diy {
namespace {

const FzConverter* const kFz_LDR_GU10_Mod[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_LDR_GU10_Mod[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_LDR_GU10_Mod[] = { "LDR-GU10-Mod" };

constexpr Expose kExposes_LDR_GU10_Mod[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired",  nullptr, nullptr, 0, ExposeCategory::State, 153, 500, 0},
};

constexpr BindingSpec kBindings_LDR_GU10_Mod[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0300},   // lightingColorCtrl
};

}  // namespace

extern const PreparedDefinition kDef_LDR_GU10_Mod{
    .zigbee_models=kModels_LDR_GU10_Mod, .zigbee_models_count=sizeof(kModels_LDR_GU10_Mod)/sizeof(kModels_LDR_GU10_Mod[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LDR-GU10-Mod", .vendor="Svetomaniya",
    .meta=nullptr, .exposes=kExposes_LDR_GU10_Mod, .exposes_count=sizeof(kExposes_LDR_GU10_Mod)/sizeof(kExposes_LDR_GU10_Mod[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_LDR_GU10_Mod, .from_zigbee_count=sizeof(kFz_LDR_GU10_Mod)/sizeof(kFz_LDR_GU10_Mod[0]),
    .to_zigbee=kTz_LDR_GU10_Mod, .to_zigbee_count=sizeof(kTz_LDR_GU10_Mod)/sizeof(kTz_LDR_GU10_Mod[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_LDR_GU10_Mod, .bindings_count=sizeof(kBindings_LDR_GU10_Mod)/sizeof(kBindings_LDR_GU10_Mod[0]),
};

}  // namespace zhc::devices::slacky_diy
