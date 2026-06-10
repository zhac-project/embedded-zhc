// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Essentielb EB-E27-A60-CCT — hand-rewritten colour light.
// E27 A60 CCT light bulb
// z2m extend: m.light({colorTemp: {range: [153, 454]}})
//   -> on/off + level + colorTemp. The auto-generator collapsed
//   m.light() to on/off + brightness only, dropping the CT axis;
//   this file restores kFzColorTemperature (+ kTzColorTemp) + the
//   color_temp expose + 0x0300 binding.
// z2m-source: essentialb.ts #EB-E27-A60-CCT.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::essentialb {
namespace {
const FzConverter* const kFz_EB_E27_A60_CCT[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_EB_E27_A60_CCT[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_EB_E27_A60_CCT[] = { "EB-E27-A60-CCT" };

}  // namespace


// Hand-curated colour-light surface.
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},  // lightingColorCtrl (CT)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_EB_E27_A60_CCT{
    .zigbee_models=kModels_EB_E27_A60_CCT, .zigbee_models_count=sizeof(kModels_EB_E27_A60_CCT)/sizeof(kModels_EB_E27_A60_CCT[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EB-E27-A60-CCT", .vendor="Essentialb",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_EB_E27_A60_CCT, .from_zigbee_count=sizeof(kFz_EB_E27_A60_CCT)/sizeof(kFz_EB_E27_A60_CCT[0]),
    .to_zigbee=kTz_EB_E27_A60_CCT, .to_zigbee_count=sizeof(kTz_EB_E27_A60_CCT)/sizeof(kTz_EB_E27_A60_CCT[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::essentialb
