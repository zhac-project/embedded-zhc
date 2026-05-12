// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: PaulNeuhaus 100.110.51 — hand-rewritten to use shared CTLight bundle.
// Q-FLAG LED panel, Smart-Home CCT
// z2m-source: paul_neuhaus.ts #100.110.51.
// z2m extend: m.light({colorTemp: {range: undefined}})
#include "definitions/_generic/_shared.hpp"
#include "definitions/paul_neuhaus/_shared.hpp"

namespace zhc::devices::paul_neuhaus {
namespace {
constexpr const char* kModels_D100_110_51[] = { "JZ-CT-Z01" };
}  // namespace

extern const PreparedDefinition kDef_D100_110_51{
    .zigbee_models=kModels_D100_110_51, .zigbee_models_count=sizeof(kModels_D100_110_51)/sizeof(kModels_D100_110_51[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="100.110.51", .vendor="PaulNeuhaus",
    .meta=nullptr, .exposes=kExposesPaulNeuhausCTLight, .exposes_count=kExposesPaulNeuhausCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzPaulNeuhausCTLight, .from_zigbee_count=kFzPaulNeuhausCTLightCount,
    .to_zigbee=kTzPaulNeuhausCTLight, .to_zigbee_count=kTzPaulNeuhausCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsPaulNeuhausCTLight, .bindings_count=kBindingsPaulNeuhausCTLightCount,
};

}  // namespace zhc::devices::paul_neuhaus
