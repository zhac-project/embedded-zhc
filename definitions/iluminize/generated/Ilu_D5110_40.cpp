// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Iluminize 5110.40 — hand-rewritten to use Ilu_ColorCTLight bundle.
// Zigbee 3.0 LED controller, 5 Ch (12V - 24V) - 4A, RGB W/CCT LED.
// z2m extend: m.light({colorTemp: {range: [160, 450]}, color: true,
//                      configureReporting: true})  →  ColorCTLight bundle.
// White-labels (5110.80, 5112.80) keyed by zigbeeModel set below.
// z2m-source: iluminize.ts #5110.40.
#include "definitions/_generic/_shared.hpp"
#include "definitions/iluminize/_shared.hpp"

namespace zhc::devices::iluminize {
namespace {

constexpr const char* kModels_D5110_40[] = {
    "HK-ZD-RGB-A", "5110.40", "5110.80", "5112.80",
};

}  // namespace

extern const PreparedDefinition kDef_D5110_40{
    .zigbee_models=kModels_D5110_40,
    .zigbee_models_count=sizeof(kModels_D5110_40)/sizeof(kModels_D5110_40[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="5110.40", .vendor="Iluminize",
    .meta=nullptr,
    .exposes=kExposesIluColorCTLight, .exposes_count=kExposesIluColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzIluColorCTLight, .from_zigbee_count=kFzIluColorCTLightCount,
    .to_zigbee=kTzIluColorCTLight, .to_zigbee_count=kTzIluColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsIluColorCTLight, .bindings_count=kBindingsIluColorCTLightCount,
};

}  // namespace zhc::devices::iluminize
