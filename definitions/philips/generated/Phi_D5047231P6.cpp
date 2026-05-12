// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips 5047231P6 — auto-generated.
// Hue White ambiance Buckram double spotlight
// z2m-source: philips.ts #5047231P6.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D5047231P6[] = { "5047231P6" };

}  // namespace

extern const PreparedDefinition kDef_D5047231P6{
    .zigbee_models           = kModels_D5047231P6,
    .zigbee_models_count     = sizeof(kModels_D5047231P6)/sizeof(kModels_D5047231P6[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "5047231P6",
    .vendor                  = "Philips",
    .meta                    = nullptr,
    .exposes                 = kExposesPhilipsCTLight,
    .exposes_count           = kExposesPhilipsCTLightCount,
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFzPhilipsCTLight,
    .from_zigbee_count       = kFzPhilipsCTLightCount,
    .to_zigbee               = kTzPhilipsCTLight,
    .to_zigbee_count         = kTzPhilipsCTLightCount,
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kBindingsPhilipsCTLight,
    .bindings_count          = kBindingsPhilipsCTLightCount,
};

}  // namespace zhc::devices::philips
