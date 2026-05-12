// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram 73699 — auto-generated.
//  Gardenspot LED mini RGB
// z2m-source: osram.ts #73699.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_D73699[] = { "Gardenspot RGB" };

}  // namespace

extern const PreparedDefinition kDef_D73699{
    .zigbee_models=kModels_D73699, .zigbee_models_count=sizeof(kModels_D73699)/sizeof(kModels_D73699[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="73699", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramColorLight, .exposes_count=kExposesOsramColorLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramColorLight, .from_zigbee_count=kFzOsramColorLightCount,
    .to_zigbee=kTzOsramColorLight, .to_zigbee_count=kTzOsramColorLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramColorLight,
    .bindings_count           = kBindingsOsramColorLightCount,
};

}  // namespace zhc::devices::osram
