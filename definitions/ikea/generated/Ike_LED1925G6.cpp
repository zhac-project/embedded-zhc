// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ikea LED1925G6 — auto-generated.
// TRADFRI bulb E12/E14/E17, color/white spectrum, globe, opal, 440/450/470 lm
// z2m-source: ikea.ts #LED1925G6.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ikea/_shared.hpp"

namespace zhc::devices::ikea {
namespace {


constexpr const char* kModels_LED1925G6[] = { "TRADFRI bulb E14 CWS 470lm", "TRADFRI bulb E12 CWS 450lm", "TRADFRI bulb E17 CWS 440lm" };

}  // namespace

extern const PreparedDefinition kDef_LED1925G6{
    .zigbee_models=kModels_LED1925G6, .zigbee_models_count=sizeof(kModels_LED1925G6)/sizeof(kModels_LED1925G6[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LED1925G6", .vendor="Ikea",
    .meta=nullptr, .exposes=kExposesIkeaColorLight, .exposes_count=kExposesIkeaColorLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzIkeaColorLight, .from_zigbee_count=kFzIkeaColorLightCount,
    .to_zigbee=kTzIkeaColorLight, .to_zigbee_count=kTzIkeaColorLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsIkeaLight,
    .bindings_count           = kBindingsIkeaLightCount,
    .reports                  = kReportsIkeaColorLight,
    .reports_count            = kReportsIkeaColorLightCount,
};

}  // namespace zhc::devices::ikea
