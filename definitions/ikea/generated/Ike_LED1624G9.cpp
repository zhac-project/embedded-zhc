// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ikea LED1624G9 — auto-generated.
// TRADFRI bulb E12/E14/E26/E27, color/white spectrum, globe, opal, 600 lm
// z2m-source: ikea.ts #LED1624G9.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ikea/_shared.hpp"

namespace zhc::devices::ikea {
namespace {


constexpr const char* kModels_LED1624G9[] = { "TRADFRI bulb E27 CWS opal 600lm", "TRADFRI bulb E26 CWS opal 600lm", "TRADFRI bulb E14 CWS opal 600lm", "TRADFRI bulb E12 CWS opal 600lm", "TRADFRI bulb E27 C/WS opal 600" };

}  // namespace

extern const PreparedDefinition kDef_LED1624G9{
    .zigbee_models=kModels_LED1624G9, .zigbee_models_count=sizeof(kModels_LED1624G9)/sizeof(kModels_LED1624G9[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LED1624G9", .vendor="Ikea",
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
