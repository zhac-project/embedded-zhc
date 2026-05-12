// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips LWG004 — auto-generated.
// Hue white GU10 bluetooth
// z2m-source: philips.ts #LWG004.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_LWG004[] = { "LWG004" };

}  // namespace

extern const PreparedDefinition kDef_LWG004{
    .zigbee_models           = kModels_LWG004,
    .zigbee_models_count     = sizeof(kModels_LWG004)/sizeof(kModels_LWG004[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "LWG004",
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
