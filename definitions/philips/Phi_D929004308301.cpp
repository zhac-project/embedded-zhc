// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: hand-authored z2m v26.76.0 parity port (Phase A).
// Philips 929004308301 — Hue Turaco outdoor wall light
// z2m-source: philips.ts #929004308301
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D929004308301[] = { "929004308301" };

}  // namespace

extern const PreparedDefinition kDef_D929004308301{
    .zigbee_models           = kModels_D929004308301,
    .zigbee_models_count     = sizeof(kModels_D929004308301)/sizeof(kModels_D929004308301[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "929004308301",
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
