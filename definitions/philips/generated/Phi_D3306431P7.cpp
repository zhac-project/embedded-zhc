// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips 3306431P7 — auto-generated.
// Hue Struana
// z2m-source: philips.ts #3306431P7.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D3306431P7[] = { "LTC012" };

}  // namespace

extern const PreparedDefinition kDef_D3306431P7{
    .zigbee_models           = kModels_D3306431P7,
    .zigbee_models_count     = sizeof(kModels_D3306431P7)/sizeof(kModels_D3306431P7[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "3306431P7",
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
