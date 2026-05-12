// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Iluminize 511.020 — hand-rewritten to use Ilu_CTLight bundle.
// Zigbee 3.0 LED controller multi 4 - 5A, CCT WW/CW LED.
// z2m extend: m.light({colorTemp: {range: [155, 450]}})  →  CTLight bundle.
// z2m-source: iluminize.ts #511.020.
#include "definitions/_generic/_shared.hpp"
#include "definitions/iluminize/_shared.hpp"

namespace zhc::devices::iluminize {
namespace {

constexpr const char* kModels_D511_020[] = { "511.020" };

}  // namespace

extern const PreparedDefinition kDef_D511_020{
    .zigbee_models=kModels_D511_020,
    .zigbee_models_count=sizeof(kModels_D511_020)/sizeof(kModels_D511_020[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="511.020", .vendor="Iluminize",
    .meta=nullptr,
    .exposes=kExposesIluCTLight, .exposes_count=kExposesIluCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzIluCTLight, .from_zigbee_count=kFzIluCTLightCount,
    .to_zigbee=kTzIluCTLight, .to_zigbee_count=kTzIluCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsIluCTLight, .bindings_count=kBindingsIluCTLightCount,
};

}  // namespace zhc::devices::iluminize
