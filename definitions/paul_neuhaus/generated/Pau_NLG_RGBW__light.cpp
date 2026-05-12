// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: PaulNeuhaus NLG-RGBW__light — hand-rewritten to use shared ColorCTLight bundle.
// Various RGBW lights (e.g. 100.111.57)
// z2m-source: paul_neuhaus.ts #NLG-RGBW__light.
// z2m extend: m.light({colorTemp: {range: undefined}, color: true})
//
// The "NLG-RGBW light" zigbeeModel (no trailing space) lives here; the
// trailing-space variant ("NLG-RGBW light ") is in Pau_NLG_RGBW_light.
#include "definitions/_generic/_shared.hpp"
#include "definitions/paul_neuhaus/_shared.hpp"

namespace zhc::devices::paul_neuhaus {
namespace {
constexpr const char* kModels_NLG_RGBW__light[] = { "NLG-RGBW light" };
}  // namespace

extern const PreparedDefinition kDef_NLG_RGBW__light{
    .zigbee_models=kModels_NLG_RGBW__light, .zigbee_models_count=sizeof(kModels_NLG_RGBW__light)/sizeof(kModels_NLG_RGBW__light[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="NLG-RGBW__light", .vendor="PaulNeuhaus",
    .meta=nullptr, .exposes=kExposesPaulNeuhausColorCTLight, .exposes_count=kExposesPaulNeuhausColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzPaulNeuhausColorCTLight, .from_zigbee_count=kFzPaulNeuhausColorCTLightCount,
    .to_zigbee=kTzPaulNeuhausColorCTLight, .to_zigbee_count=kTzPaulNeuhausColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsPaulNeuhausColorCTLight, .bindings_count=kBindingsPaulNeuhausColorCTLightCount,
};

}  // namespace zhc::devices::paul_neuhaus
