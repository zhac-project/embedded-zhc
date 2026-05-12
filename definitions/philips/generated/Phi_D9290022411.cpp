// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips 9290022411 — auto-generated.
// Hue white single filament bulb A19 E26 with Bluetooth
// z2m-source: philips.ts #9290022411.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D9290022411[] = { "LWA005" };

}  // namespace

extern const PreparedDefinition kDef_D9290022411{
    .zigbee_models           = kModels_D9290022411,
    .zigbee_models_count     = sizeof(kModels_D9290022411)/sizeof(kModels_D9290022411[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "9290022411",
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
