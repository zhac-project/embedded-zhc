// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ikea LED2004G8 — auto-generated.
// TRADFRI bulb E26/E27, white spectrum, globe, clear, 800/806/810 lm
// z2m-source: ikea.ts #LED2004G8.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ikea/_shared.hpp"

namespace zhc::devices::ikea {
namespace {


constexpr const char* kModels_LED2004G8[] = { "TRADFRIbulbE26WSglobeclear800lm", "TRADFRIbulbE27WSglobeclear806lm", "TRADFRIbulbE26WSglobeclear806lm", "TRADFRIbulbE26WSglobeclear810lm" };

}  // namespace

extern const PreparedDefinition kDef_LED2004G8{
    .zigbee_models=kModels_LED2004G8, .zigbee_models_count=sizeof(kModels_LED2004G8)/sizeof(kModels_LED2004G8[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LED2004G8", .vendor="Ikea",
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
