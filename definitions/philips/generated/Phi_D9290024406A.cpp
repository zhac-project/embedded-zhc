// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips 9290024406A — auto-generated.
// Hue P45 light bulb
// z2m-source: philips.ts #9290024406A.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D9290024406A[] = { "LWU002" };

}  // namespace

extern const PreparedDefinition kDef_D9290024406A{
    .zigbee_models           = kModels_D9290024406A,
    .zigbee_models_count     = sizeof(kModels_D9290024406A)/sizeof(kModels_D9290024406A[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "9290024406A",
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
