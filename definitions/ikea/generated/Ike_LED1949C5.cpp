// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ikea LED1949C5 — auto-generated.
// TRADFRI bulb E12/E14/E17, white spectrum, candle, opal, 450/470/440 lm
// z2m-source: ikea.ts #LED1949C5.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ikea/_shared.hpp"

namespace zhc::devices::ikea {
namespace {


constexpr const char* kModels_LED1949C5[] = { "TRADFRIbulbE14WScandleopal470lm", "TRADFRIbulbE12WScandleopal450lm", "TRADFRIbulbE17WScandleopal440lm" };

}  // namespace

extern const PreparedDefinition kDef_LED1949C5{
    .zigbee_models=kModels_LED1949C5, .zigbee_models_count=sizeof(kModels_LED1949C5)/sizeof(kModels_LED1949C5[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LED1949C5", .vendor="Ikea",
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
