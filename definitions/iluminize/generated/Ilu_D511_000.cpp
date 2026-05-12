// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Iluminize 511.000 — hand-rewritten to use Ilu_ColorCTLight bundle.
// Zigbee 3.0 universal LED-controller, 5 channel, RGBCCT LED.
// z2m extend: m.light({colorTemp: {range: undefined}, color: true,
//                      configureReporting: true})  →  ColorCTLight bundle.
// White-label Sunricher HK-ZD-RGBCCT-A is carried in the zigbeeModel set.
// z2m-source: iluminize.ts #511.000.
#include "definitions/_generic/_shared.hpp"
#include "definitions/iluminize/_shared.hpp"

namespace zhc::devices::iluminize {
namespace {

constexpr const char* kModels_D511_000[] = { "HK-ZD-RGBCCT-A", "511.000" };

}  // namespace

extern const PreparedDefinition kDef_D511_000{
    .zigbee_models=kModels_D511_000,
    .zigbee_models_count=sizeof(kModels_D511_000)/sizeof(kModels_D511_000[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="511.000", .vendor="Iluminize",
    .meta=nullptr,
    .exposes=kExposesIluColorCTLight, .exposes_count=kExposesIluColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzIluColorCTLight, .from_zigbee_count=kFzIluColorCTLightCount,
    .to_zigbee=kTzIluColorCTLight, .to_zigbee_count=kTzIluColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsIluColorCTLight, .bindings_count=kBindingsIluColorCTLightCount,
};

}  // namespace zhc::devices::iluminize
