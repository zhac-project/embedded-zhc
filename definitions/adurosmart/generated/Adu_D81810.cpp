// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Adurosmart 81810 — hand-rewritten to use Adu_Light bundle.
// Zigbee Aduro Eria B22 bulb - warm white.
// z2m extend: m.light().
// z2m-source: adurosmart.ts #81810.
#include "definitions/_generic/_shared.hpp"
#include "definitions/adurosmart/_shared.hpp"

namespace zhc::devices::adurosmart {
namespace {

constexpr const char* kModels_D81810[] = { "AD-DimmableLight3001" };

}  // namespace

extern const PreparedDefinition kDef_D81810{
    .zigbee_models=kModels_D81810,
    .zigbee_models_count=sizeof(kModels_D81810)/sizeof(kModels_D81810[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="81810", .vendor="Adurosmart",
    .meta=nullptr,
    .exposes=kExposesAduLight, .exposes_count=kExposesAduLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAduLight, .from_zigbee_count=kFzAduLightCount,
    .to_zigbee=kTzAduLight, .to_zigbee_count=kTzAduLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAduLight, .bindings_count=kBindingsAduLightCount,
};

}  // namespace zhc::devices::adurosmart
