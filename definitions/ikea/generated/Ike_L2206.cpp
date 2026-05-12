// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ikea L2206 — auto-generated.
// JETSTROM ceiling light panel, color/white spectrum, 30x30 cm
// z2m-source: ikea.ts #L2206.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ikea/_shared.hpp"

namespace zhc::devices::ikea {
namespace {


constexpr const char* kModels_L2206[] = { "JETSTROM 3030 ceiling" };

}  // namespace

extern const PreparedDefinition kDef_L2206{
    .zigbee_models=kModels_L2206, .zigbee_models_count=sizeof(kModels_L2206)/sizeof(kModels_L2206[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="L2206", .vendor="Ikea",
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
