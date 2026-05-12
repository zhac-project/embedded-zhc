// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips LP_CF_7904008_EU — auto-generated.
// MasterConnect LEDtube EM/mains T8
// z2m-source: philips.ts #LP_CF_7904008_EU.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_LP_CF_7904008_EU[] = { "LEDtube T8 5FT V2.0" };

}  // namespace

extern const PreparedDefinition kDef_LP_CF_7904008_EU{
    .zigbee_models           = kModels_LP_CF_7904008_EU,
    .zigbee_models_count     = sizeof(kModels_LP_CF_7904008_EU)/sizeof(kModels_LP_CF_7904008_EU[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "LP_CF_7904008_EU",
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
