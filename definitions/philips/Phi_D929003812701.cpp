// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: hand-authored z2m v26.76.0 parity port (Phase A).
// Philips 929003812701 — Hue Xamento White and Color Ambiance GU10 (Black)
// z2m-source: philips.ts #929003812701
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D929003812701[] = { "929003812701_01", "929003812701_02", "929003812701_03" };

}  // namespace

extern const PreparedDefinition kDef_D929003812701{
    .zigbee_models           = kModels_D929003812701,
    .zigbee_models_count     = sizeof(kModels_D929003812701)/sizeof(kModels_D929003812701[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "929003812701",
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
