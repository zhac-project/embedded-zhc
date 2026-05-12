// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ecosmart D1821 — uses shared ColorCTLight bundle.
// A19 RGB bulb
// z2m-source: ecosmart.ts #D1821 — m.light({colorTemp: {range: undefined}, color: true}).
#include "definitions/_generic/_shared.hpp"
#include "definitions/ecosmart/_shared.hpp"

namespace zhc::devices::ecosmart {
namespace {
constexpr const char* kModels_D1821[] = { "zhaRGBW" };
}  // namespace

extern const PreparedDefinition kDef_D1821{
    .zigbee_models=kModels_D1821,
    .zigbee_models_count=sizeof(kModels_D1821)/sizeof(kModels_D1821[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="D1821", .vendor="Ecosmart",
    .meta=nullptr,
    .exposes=kExposesEcosmartColorCTLight, .exposes_count=kExposesEcosmartColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzEcosmartColorCTLight, .from_zigbee_count=kFzEcosmartColorCTLightCount,
    .to_zigbee=kTzEcosmartColorCTLight,   .to_zigbee_count=kTzEcosmartColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsEcosmartColorCTLight, .bindings_count=kBindingsEcosmartColorCTLightCount,
};

}  // namespace zhc::devices::ecosmart
