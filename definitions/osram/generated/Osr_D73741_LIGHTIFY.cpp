// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram 73741_LIGHTIFY — auto-generated.
// LIGHTIFY RT5/6 LED
// z2m-source: osram.ts #73741_LIGHTIFY.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_D73741_LIGHTIFY[] = { "LIGHTIFY RT RGBW" };

}  // namespace

extern const PreparedDefinition kDef_D73741_LIGHTIFY{
    .zigbee_models=kModels_D73741_LIGHTIFY, .zigbee_models_count=sizeof(kModels_D73741_LIGHTIFY)/sizeof(kModels_D73741_LIGHTIFY[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="73741_LIGHTIFY", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramColorCTLight, .exposes_count=kExposesOsramColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramColorCTLight, .from_zigbee_count=kFzOsramColorCTLightCount,
    .to_zigbee=kTzOsramColorCTLight, .to_zigbee_count=kTzOsramColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramColorCTLight,
    .bindings_count           = kBindingsOsramColorCTLightCount,
};

}  // namespace zhc::devices::osram
