// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: hand-authored z2m v26.76.0 parity port (Phase A).
// Philips 929004308401 — Hue White and Color Ambiance Lucca Outdoor wall light
// z2m-source: philips.ts #929004308401
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D929004308401[] = { "929004308401" };

}  // namespace

extern const PreparedDefinition kDef_D929004308401{
    .zigbee_models           = kModels_D929004308401,
    .zigbee_models_count     = sizeof(kModels_D929004308401)/sizeof(kModels_D929004308401[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "929004308401",
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
};

}  // namespace zhc::devices::philips
