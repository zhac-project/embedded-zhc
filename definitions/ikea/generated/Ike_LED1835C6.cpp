// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ikea LED1835C6 — auto-generated.
// TRADFRI bulb E12/E14/E17, white spectrum, candle, opal, 450/470/440 lm
// z2m-source: ikea.ts #LED1835C6.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ikea/_shared.hpp"

namespace zhc::devices::ikea {
namespace {


constexpr const char* kModels_LED1835C6[] = { "TRADFRI bulb E14 WS 470lm", "TRADFRI bulb E12 WS 450lm", "TRADFRI bulb E17 WS 440lm", "TRADFRI bulb E17 WS candle 440lm" };

}  // namespace

extern const PreparedDefinition kDef_LED1835C6{
    .zigbee_models=kModels_LED1835C6, .zigbee_models_count=sizeof(kModels_LED1835C6)/sizeof(kModels_LED1835C6[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LED1835C6", .vendor="Ikea",
    .meta=nullptr, .exposes=kExposesIkeaLight, .exposes_count=kExposesIkeaLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzIkeaLight, .from_zigbee_count=kFzIkeaLightCount,
    .to_zigbee=kTzIkeaLight, .to_zigbee_count=kTzIkeaLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsIkeaLight,
    .bindings_count           = kBindingsIkeaLightCount,
    .reports                  = kReportsIkeaLight,
    .reports_count            = kReportsIkeaLightCount,
};

}  // namespace zhc::devices::ikea
