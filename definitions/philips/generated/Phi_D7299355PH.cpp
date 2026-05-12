// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips 7299355PH — auto-generated.
// Hue white and color ambiance LightStrip
// z2m-source: philips.ts #7299355PH.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D7299355PH[] = { "LST001" };

}  // namespace

extern const PreparedDefinition kDef_D7299355PH{
    .zigbee_models           = kModels_D7299355PH,
    .zigbee_models_count     = sizeof(kModels_D7299355PH)/sizeof(kModels_D7299355PH[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "7299355PH",
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
