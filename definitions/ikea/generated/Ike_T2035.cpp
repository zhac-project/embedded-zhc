// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ikea T2035 — auto-generated.
// STOFTMOLN ceiling/wall lamp, warm white, 24 cm
// z2m-source: ikea.ts #T2035.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ikea/_shared.hpp"

namespace zhc::devices::ikea {
namespace {


constexpr const char* kModels_T2035[] = { "STOFTMOLN ceiling/wall lamp WW24" };

}  // namespace

extern const PreparedDefinition kDef_T2035{
    .zigbee_models=kModels_T2035, .zigbee_models_count=sizeof(kModels_T2035)/sizeof(kModels_T2035[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="T2035", .vendor="Ikea",
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
