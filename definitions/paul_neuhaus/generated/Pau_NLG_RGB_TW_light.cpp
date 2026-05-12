// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: PaulNeuhaus NLG-RGB-TW light — hand-rewritten to use shared ColorCTLight bundle.
// Various RGB + tunable white lights (e.g. 100.470.92)
// z2m-source: paul_neuhaus.ts #NLG-RGB-TW light.
// z2m extend: m.light({colorTemp: {range: undefined}, color: true})
#include "definitions/_generic/_shared.hpp"
#include "definitions/paul_neuhaus/_shared.hpp"

namespace zhc::devices::paul_neuhaus {
namespace {
constexpr const char* kModels_NLG_RGB_TW_light[] = { "NLG-RGB-TW light" };
}  // namespace

extern const PreparedDefinition kDef_NLG_RGB_TW_light{
    .zigbee_models=kModels_NLG_RGB_TW_light, .zigbee_models_count=sizeof(kModels_NLG_RGB_TW_light)/sizeof(kModels_NLG_RGB_TW_light[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="NLG-RGB-TW light", .vendor="PaulNeuhaus",
    .meta=nullptr, .exposes=kExposesPaulNeuhausColorCTLight, .exposes_count=kExposesPaulNeuhausColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzPaulNeuhausColorCTLight, .from_zigbee_count=kFzPaulNeuhausColorCTLightCount,
    .to_zigbee=kTzPaulNeuhausColorCTLight, .to_zigbee_count=kTzPaulNeuhausColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsPaulNeuhausColorCTLight, .bindings_count=kBindingsPaulNeuhausColorCTLightCount,
};

}  // namespace zhc::devices::paul_neuhaus
