// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: PaulNeuhaus 100.001.96 — hand-rewritten to use shared ColorCTLight bundle.
// Q-LED Lamp RGBW E27 socket
// z2m-source: paul_neuhaus.ts #100.001.96.
// z2m extend: m.light({colorTemp: {range: undefined}, color: true})
#include "definitions/_generic/_shared.hpp"
#include "definitions/paul_neuhaus/_shared.hpp"

namespace zhc::devices::paul_neuhaus {
namespace {
constexpr const char* kModels_D100_001_96[] = { "JZD60-J4R150" };
}  // namespace

extern const PreparedDefinition kDef_D100_001_96{
    .zigbee_models=kModels_D100_001_96, .zigbee_models_count=sizeof(kModels_D100_001_96)/sizeof(kModels_D100_001_96[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="100.001.96", .vendor="PaulNeuhaus",
    .meta=nullptr, .exposes=kExposesPaulNeuhausColorCTLight, .exposes_count=kExposesPaulNeuhausColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzPaulNeuhausColorCTLight, .from_zigbee_count=kFzPaulNeuhausColorCTLightCount,
    .to_zigbee=kTzPaulNeuhausColorCTLight, .to_zigbee_count=kTzPaulNeuhausColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsPaulNeuhausColorCTLight, .bindings_count=kBindingsPaulNeuhausColorCTLightCount,
};

}  // namespace zhc::devices::paul_neuhaus
