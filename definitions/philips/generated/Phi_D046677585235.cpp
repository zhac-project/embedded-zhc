// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips 046677585235 — auto-generated.
// Hue Dymera indoor and outdoor wall light
// z2m-source: philips.ts #046677585235.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D046677585235[] = { "LCW004", "LCW005" };

constexpr ::zhc::EndpointLabel kEndpoints_D046677585235[] = { {"top", 1}, {"bottom", 2} };

}  // namespace

extern const PreparedDefinition kDef_D046677585235{
    .zigbee_models           = kModels_D046677585235,
    .zigbee_models_count     = sizeof(kModels_D046677585235)/sizeof(kModels_D046677585235[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "046677585235",
    .vendor                  = "Philips",
    .meta                    = nullptr,
    .exposes                 = kExposesPhilipsColorCTLight,
    .exposes_count           = kExposesPhilipsColorCTLightCount,
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFzPhilipsColorCTLight,
    .from_zigbee_count       = kFzPhilipsColorCTLightCount,
    .to_zigbee               = kTzPhilipsColorCTLight,
    .to_zigbee_count         = kTzPhilipsColorCTLightCount,
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kBindingsPhilipsCTLight,
    .bindings_count          = kBindingsPhilipsCTLightCount,
    .endpoint_map       = kEndpoints_D046677585235,
    .endpoint_map_count = sizeof(kEndpoints_D046677585235)/sizeof(kEndpoints_D046677585235[0]),
};

}  // namespace zhc::devices::philips
