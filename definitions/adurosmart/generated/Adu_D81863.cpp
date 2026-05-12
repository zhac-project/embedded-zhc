// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Adurosmart 81863 — hand-rewritten to use Adu_ColorCTLight bundle.
// Eria color LED strip.
// z2m extend: m.light({colorTemp: {range: [153, 500]}, color: {modes: ["xy", "hs"], applyRedFix: true}}).
// z2m-source: adurosmart.ts #81863.
#include "definitions/_generic/_shared.hpp"
#include "definitions/adurosmart/_shared.hpp"

namespace zhc::devices::adurosmart {
namespace {

constexpr const char* kModels_D81863[] = { "Extended Color LED Strip V1.0" };

}  // namespace

extern const PreparedDefinition kDef_D81863{
    .zigbee_models=kModels_D81863,
    .zigbee_models_count=sizeof(kModels_D81863)/sizeof(kModels_D81863[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="81863", .vendor="Adurosmart",
    .meta=nullptr,
    .exposes=kExposesAduColorCTLight, .exposes_count=kExposesAduColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAduColorCTLight, .from_zigbee_count=kFzAduColorCTLightCount,
    .to_zigbee=kTzAduColorCTLight, .to_zigbee_count=kTzAduColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAduColorCTLight, .bindings_count=kBindingsAduColorCTLightCount,
};

}  // namespace zhc::devices::adurosmart
