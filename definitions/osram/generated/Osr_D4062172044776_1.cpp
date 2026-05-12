// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram 4062172044776_1 — auto-generated.
// Zigbee 3.0 DALI CONV LI dimmer for DALI-based luminaires (only one device)
// z2m-source: osram.ts #4062172044776_1.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_D4062172044776_1[] = { "Zigbee 3.0 DALI CONV LI" };

}  // namespace

extern const PreparedDefinition kDef_D4062172044776_1{
    .zigbee_models=kModels_D4062172044776_1, .zigbee_models_count=sizeof(kModels_D4062172044776_1)/sizeof(kModels_D4062172044776_1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4062172044776_1", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramCTLight, .exposes_count=kExposesOsramCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramCTLight, .from_zigbee_count=kFzOsramCTLightCount,
    .to_zigbee=kTzOsramCTLight, .to_zigbee_count=kTzOsramCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramCTLight,
    .bindings_count           = kBindingsOsramCTLightCount,
};

}  // namespace zhc::devices::osram
