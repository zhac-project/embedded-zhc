// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Leedarson A806S-Q1G — hand-rewritten 2026-04-29.
// LED E27 color
// z2m-source: leedarson.ts #A806S-Q1G.
// Mapping: m.light({colorTemp: {range: undefined}, color: true})
//          -> kFz/kTz/kExposes/kBindings LeedarsonColorCTLight (full RGBW).
#include "definitions/_generic/_shared.hpp"
#include "definitions/leedarson/_shared.hpp"

namespace zhc::devices::leedarson {
namespace {
constexpr const char* kModels_A806S_Q1G[] = { "LED_E27_ORD" };
}  // namespace

extern const PreparedDefinition kDef_A806S_Q1G{
    .zigbee_models=kModels_A806S_Q1G, .zigbee_models_count=sizeof(kModels_A806S_Q1G)/sizeof(kModels_A806S_Q1G[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="A806S-Q1G", .vendor="Leedarson",
    .meta=nullptr,
    .exposes=kExposesLeedarsonColorCTLight, .exposes_count=kExposesLeedarsonColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLeedarsonColorCTLight, .from_zigbee_count=kFzLeedarsonColorCTLightCount,
    .to_zigbee=kTzLeedarsonColorCTLight, .to_zigbee_count=kTzLeedarsonColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsLeedarsonColorCTLight, .bindings_count=kBindingsLeedarsonColorCTLightCount,
};

}  // namespace zhc::devices::leedarson
