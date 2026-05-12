// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: PaulNeuhaus 100.075.74 — hand-rewritten to use shared ColorCTLight bundle.
// Q-VIDAL RGBW ceiling lamp, 6032-55
// z2m-source: paul_neuhaus.ts #100.075.74.
// z2m extend: m.light({colorTemp: {range: undefined}, color: true})
// z2m endpoint: () => ({default: 2})
//
// PARTIAL: bindings overridden to EP2 inline; default-EP routing for
// commands not yet expressible on PreparedDefinition.
#include "definitions/_generic/_shared.hpp"
#include "definitions/paul_neuhaus/_shared.hpp"

namespace zhc::devices::paul_neuhaus {
namespace {
constexpr const char* kModels_D100_075_74[] = { "JZ-RGBW-Z01" };

constexpr ::zhc::BindingSpec kBindings_D100_075_74[] = {
    { 2, 0x0006 }, { 2, 0x0008 }, { 2, 0x0300 },
};
}  // namespace

extern const PreparedDefinition kDef_D100_075_74{
    .zigbee_models=kModels_D100_075_74, .zigbee_models_count=sizeof(kModels_D100_075_74)/sizeof(kModels_D100_075_74[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="100.075.74", .vendor="PaulNeuhaus",
    .meta=nullptr, .exposes=kExposesPaulNeuhausColorCTLight, .exposes_count=kExposesPaulNeuhausColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzPaulNeuhausColorCTLight, .from_zigbee_count=kFzPaulNeuhausColorCTLightCount,
    .to_zigbee=kTzPaulNeuhausColorCTLight, .to_zigbee_count=kTzPaulNeuhausColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D100_075_74, .bindings_count=sizeof(kBindings_D100_075_74)/sizeof(kBindings_D100_075_74[0]),
    .default_endpoint = 2,
};

}  // namespace zhc::devices::paul_neuhaus
