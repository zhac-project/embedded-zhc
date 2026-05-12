// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips 929003711201 — auto-generated.
// Hue Twilight sleep and wake-up light white
// z2m-source: philips.ts #929003711201.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D929003711201[] = { "LGT001", "LGT002", "LGT003" };

constexpr ::zhc::EndpointLabel kEndpoints_D929003711201[] = { {"switch", 1}, {"back", 1}, {"front", 1} };

}  // namespace

extern const PreparedDefinition kDef_D929003711201{
    .zigbee_models           = kModels_D929003711201,
    .zigbee_models_count     = sizeof(kModels_D929003711201)/sizeof(kModels_D929003711201[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "929003711201",
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
    .endpoint_map       = kEndpoints_D929003711201,
    .endpoint_map_count = sizeof(kEndpoints_D929003711201)/sizeof(kEndpoints_D929003711201[0]),
};

}  // namespace zhc::devices::philips
