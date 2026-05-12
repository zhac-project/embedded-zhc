// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ikea LED1937T5 — auto-generated.
// TRADFRI bulb E26/E27, white spectrum, T120 cylinder, opal, 440/450/470 lm
// z2m-source: ikea.ts #LED1937T5.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ikea/_shared.hpp"

namespace zhc::devices::ikea {
namespace {


constexpr const char* kModels_LED1937T5[] = { "TRADFRI bulb E27 opal 470lm", "TRADFRI bulb E27 W opal 470lm", "TRADFRIbulbT120E27WSopal470lm", "TRADFRIbulbT120E26WSopal450lm", "TRADFRIbulbT120E26WSopal470lm", "TRADFRI bulb E26 WS opal 440lm", "TRADFRIbulbT120E26WSopal440lm" };

}  // namespace

extern const PreparedDefinition kDef_LED1937T5{
    .zigbee_models=kModels_LED1937T5, .zigbee_models_count=sizeof(kModels_LED1937T5)/sizeof(kModels_LED1937T5[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LED1937T5", .vendor="Ikea",
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
