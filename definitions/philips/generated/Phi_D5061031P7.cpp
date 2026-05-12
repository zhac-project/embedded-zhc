// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips 5061031P7 — auto-generated.
// Hue White & Color ambience Centris ceiling light (2 spots) (white)
// z2m-source: philips.ts #5061031P7.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D5061031P7[] = { "5061031P7_01", "5061031P7_02", "5061031P7_03", "929003621001_01", "929003621001_02", "929003621001_03" };

}  // namespace

extern const PreparedDefinition kDef_D5061031P7{
    .zigbee_models           = kModels_D5061031P7,
    .zigbee_models_count     = sizeof(kModels_D5061031P7)/sizeof(kModels_D5061031P7[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "5061031P7",
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
