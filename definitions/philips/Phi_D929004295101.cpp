// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Philips 929004295101 Hue Essential Flex lightstrip 5m (z2m v26.125.0
// window). Same surface as 929004294901: philips.m.light({colorTemp: [153, 447],
// color: xy + hs, enhancedHue}) + m.identify().
// z2m-source: philips.ts #929004295101
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D929004295101[] = { "929004295101" };

}  // namespace

extern const PreparedDefinition kDef_D929004295101{
    .zigbee_models           = kModels_D929004295101,
    .zigbee_models_count     = sizeof(kModels_D929004295101)/sizeof(kModels_D929004295101[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "929004295101",
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
