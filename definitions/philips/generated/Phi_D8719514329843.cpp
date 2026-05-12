// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips 8719514329843 — auto-generated.
// Hue A60 - E27 smart bulb - 800 lumen
// z2m-source: philips.ts #8719514329843.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D8719514329843[] = { "LWF004" };

}  // namespace

extern const PreparedDefinition kDef_D8719514329843{
    .zigbee_models           = kModels_D8719514329843,
    .zigbee_models_count     = sizeof(kModels_D8719514329843)/sizeof(kModels_D8719514329843[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "8719514329843",
    .vendor                  = "Philips",
    .meta                    = nullptr,
    .exposes                 = kExposesPhilipsLight,
    .exposes_count           = kExposesPhilipsLightCount,
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFzPhilipsLight,
    .from_zigbee_count       = kFzPhilipsLightCount,
    .to_zigbee               = kTzPhilipsLight,
    .to_zigbee_count         = kTzPhilipsLightCount,
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kBindingsPhilipsLight,
    .bindings_count          = kBindingsPhilipsLightCount,
};

}  // namespace zhc::devices::philips
