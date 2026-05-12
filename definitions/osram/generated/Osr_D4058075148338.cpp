// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram 4058075148338 — auto-generated.
// LIGHTIFY LED PAR16 50 GU10 tunable white
// z2m-source: osram.ts #4058075148338.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_D4058075148338[] = { "PAR16 TW Z3" };

}  // namespace

extern const PreparedDefinition kDef_D4058075148338{
    .zigbee_models=kModels_D4058075148338, .zigbee_models_count=sizeof(kModels_D4058075148338)/sizeof(kModels_D4058075148338[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4058075148338", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramCTLight, .exposes_count=kExposesOsramCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramCTLight, .from_zigbee_count=kFzOsramCTLightCount,
    .to_zigbee=kTzOsramCTLight, .to_zigbee_count=kTzOsramCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramCTLight,
    .bindings_count           = kBindingsOsramCTLightCount,
};

}  // namespace zhc::devices::osram
