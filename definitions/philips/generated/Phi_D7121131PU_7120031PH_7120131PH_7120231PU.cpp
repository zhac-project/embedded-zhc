// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips 7121131PU/7120031PH/7120131PH/7120231PU — auto-generated.
// Hue Beyond white and color ambiance suspension/ceiling/table lamp
// z2m-source: philips.ts #7121131PU/7120031PH/7120131PH/7120231PU.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D7121131PU_7120031PH_7120131PH_7120231PU[] = { "LLM001" };

}  // namespace

extern const PreparedDefinition kDef_D7121131PU_7120031PH_7120131PH_7120231PU{
    .zigbee_models           = kModels_D7121131PU_7120031PH_7120131PH_7120231PU,
    .zigbee_models_count     = sizeof(kModels_D7121131PU_7120031PH_7120131PH_7120231PU)/sizeof(kModels_D7121131PU_7120031PH_7120131PH_7120231PU[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "7121131PU/7120031PH/7120131PH/7120231PU",
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
