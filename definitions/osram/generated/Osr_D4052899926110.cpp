// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram 4052899926110 — auto-generated.
// Flex RGBW
// z2m-source: osram.ts #4052899926110.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_D4052899926110[] = { "Flex RGBW", "LIGHTIFY Indoor Flex RGBW", "LIGHTIFY Flex RGBW" };

}  // namespace

extern const PreparedDefinition kDef_D4052899926110{
    .zigbee_models=kModels_D4052899926110, .zigbee_models_count=sizeof(kModels_D4052899926110)/sizeof(kModels_D4052899926110[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4052899926110", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramColorCTLight, .exposes_count=kExposesOsramColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramColorCTLight, .from_zigbee_count=kFzOsramColorCTLightCount,
    .to_zigbee=kTzOsramColorCTLight, .to_zigbee_count=kTzOsramColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramColorCTLight,
    .bindings_count           = kBindingsOsramColorCTLightCount,
};

}  // namespace zhc::devices::osram
