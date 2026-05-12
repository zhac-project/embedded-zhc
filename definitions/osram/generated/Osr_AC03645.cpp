// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram AC03645 — auto-generated.
// LIGHTIFY LED CLA60 E27 RGBW
// z2m-source: osram.ts #AC03645.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_AC03645[] = { "CLA60 RGBW OSRAM" };

}  // namespace

extern const PreparedDefinition kDef_AC03645{
    .zigbee_models=kModels_AC03645, .zigbee_models_count=sizeof(kModels_AC03645)/sizeof(kModels_AC03645[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AC03645", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramColorCTLight, .exposes_count=kExposesOsramColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramColorCTLight, .from_zigbee_count=kFzOsramColorCTLightCount,
    .to_zigbee=kTzOsramColorCTLight, .to_zigbee_count=kTzOsramColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramColorCTLight,
    .bindings_count           = kBindingsOsramColorCTLightCount,
};

}  // namespace zhc::devices::osram
