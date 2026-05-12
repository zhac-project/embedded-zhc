// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips 7099930PH — auto-generated.
// Hue Iris (Generation 2)
// z2m-source: philips.ts #7099930PH.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D7099930PH[] = { "LLC006" };

}  // namespace

extern const PreparedDefinition kDef_D7099930PH{
    .zigbee_models           = kModels_D7099930PH,
    .zigbee_models_count     = sizeof(kModels_D7099930PH)/sizeof(kModels_D7099930PH[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "7099930PH",
    .vendor                  = "Philips",
    .meta                    = nullptr,
    .exposes                 = kExposesPhilipsColorLight,
    .exposes_count           = kExposesPhilipsColorLightCount,
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFzPhilipsColorLight,
    .from_zigbee_count       = kFzPhilipsColorLightCount,
    .to_zigbee               = kTzPhilipsColorLight,
    .to_zigbee_count         = kTzPhilipsColorLightCount,
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kBindingsPhilipsCTLight,
    .bindings_count          = kBindingsPhilipsCTLightCount,
};

}  // namespace zhc::devices::philips
