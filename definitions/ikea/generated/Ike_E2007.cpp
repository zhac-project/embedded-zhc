// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ikea E2007 — auto-generated.
// STARKVIND air purifier
// z2m-source: ikea.ts #E2007.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ikea/_shared.hpp"

namespace zhc::devices::ikea {
namespace {


constexpr const char* kModels_E2007[] = { "STARKVIND Air purifier", "STARKVIND Air purifier table" };

}  // namespace


constexpr WhiteLabel kWhiteLabels_E2007[] = {
    {"IKEA","E2006"},
};
extern const PreparedDefinition kDef_E2007{
    .zigbee_models=kModels_E2007, .zigbee_models_count=sizeof(kModels_E2007)/sizeof(kModels_E2007[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E2007", .vendor="Ikea",
    .meta=nullptr, .exposes=kExposesIkeaLight, .exposes_count=kExposesIkeaLightCount,
    .white_labels=kWhiteLabels_E2007, .white_labels_count=sizeof(kWhiteLabels_E2007)/sizeof(kWhiteLabels_E2007[0]),
    .from_zigbee=kFzIkeaLight, .from_zigbee_count=kFzIkeaLightCount,
    .to_zigbee=kTzIkeaLight, .to_zigbee_count=kTzIkeaLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsIkeaLight,
    .bindings_count           = kBindingsIkeaLightCount,
    .reports                  = kReportsIkeaLight,
    .reports_count            = kReportsIkeaLightCount,
};

}  // namespace zhc::devices::ikea
