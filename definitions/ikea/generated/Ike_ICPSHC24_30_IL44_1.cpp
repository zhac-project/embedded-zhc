// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ikea ICPSHC24-30-IL44-1 — auto-generated.
// SILVERGLANS LED driver, 30 w, IP44
// z2m-source: ikea.ts #ICPSHC24-30-IL44-1.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ikea/_shared.hpp"

namespace zhc::devices::ikea {
namespace {


constexpr const char* kModels_ICPSHC24_30_IL44_1[] = { "SILVERGLANS IP44 LED driver" };

}  // namespace

extern const PreparedDefinition kDef_ICPSHC24_30_IL44_1{
    .zigbee_models=kModels_ICPSHC24_30_IL44_1, .zigbee_models_count=sizeof(kModels_ICPSHC24_30_IL44_1)/sizeof(kModels_ICPSHC24_30_IL44_1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ICPSHC24-30-IL44-1", .vendor="Ikea",
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
