// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ecosmart D1542 — uses shared CTLight bundle.
// GU10 adjustable white bulb
// z2m-source: ecosmart.ts #D1542 — m.light({colorTemp: {range: undefined}}).
#include "definitions/_generic/_shared.hpp"
#include "definitions/ecosmart/_shared.hpp"

namespace zhc::devices::ecosmart {
namespace {
constexpr const char* kModels_D1542[] = { "zhaTunW" };
}  // namespace

extern const PreparedDefinition kDef_D1542{
    .zigbee_models=kModels_D1542,
    .zigbee_models_count=sizeof(kModels_D1542)/sizeof(kModels_D1542[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="D1542", .vendor="Ecosmart",
    .meta=nullptr,
    .exposes=kExposesEcosmartCTLight, .exposes_count=kExposesEcosmartCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzEcosmartCTLight, .from_zigbee_count=kFzEcosmartCTLightCount,
    .to_zigbee=kTzEcosmartCTLight,   .to_zigbee_count=kTzEcosmartCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsEcosmartCTLight, .bindings_count=kBindingsEcosmartCTLightCount,
};

}  // namespace zhc::devices::ecosmart
