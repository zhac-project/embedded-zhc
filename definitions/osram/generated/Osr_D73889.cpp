// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram 73889 — auto-generated.
// Smart home soft white PAR38 outdoor bulb
// z2m-source: osram.ts #73889.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_D73889[] = { "LIGHTIFY PAR38 ON/OFF/DIM" };

}  // namespace

extern const PreparedDefinition kDef_D73889{
    .zigbee_models=kModels_D73889, .zigbee_models_count=sizeof(kModels_D73889)/sizeof(kModels_D73889[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="73889", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramLight, .exposes_count=kExposesOsramLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramLight, .from_zigbee_count=kFzOsramLightCount,
    .to_zigbee=kTzOsramLight, .to_zigbee_count=kTzOsramLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramLight,
    .bindings_count           = kBindingsOsramLightCount,
};

}  // namespace zhc::devices::osram
