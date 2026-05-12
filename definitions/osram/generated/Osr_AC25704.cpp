// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram AC25704 — auto-generated.
// Classic E14 tunable white
// z2m-source: osram.ts #AC25704.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_AC25704[] = { "B40 TW Value" };

}  // namespace

extern const PreparedDefinition kDef_AC25704{
    .zigbee_models=kModels_AC25704, .zigbee_models_count=sizeof(kModels_AC25704)/sizeof(kModels_AC25704[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AC25704", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramCTLight, .exposes_count=kExposesOsramCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramCTLight, .from_zigbee_count=kFzOsramCTLightCount,
    .to_zigbee=kTzOsramCTLight, .to_zigbee_count=kTzOsramCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramCTLight,
    .bindings_count           = kBindingsOsramCTLightCount,
};

}  // namespace zhc::devices::osram
