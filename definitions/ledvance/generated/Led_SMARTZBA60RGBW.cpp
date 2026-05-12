// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ledvance SMARTZBA60RGBW — auto-generated.
// SMART+ lamp B22D RGBTW
// Bundle: ColorCTLight (RGB + tunable white).
// z2m-source: ledvance.ts #SMARTZBA60RGBW.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ledvance/_shared.hpp"

namespace zhc::devices::ledvance {
namespace {

constexpr const char* kModels_SMARTZBA60RGBW[] = { "CLA60 RGBW JP" };

}  // namespace

extern const PreparedDefinition kDef_SMARTZBA60RGBW{
    .zigbee_models=kModels_SMARTZBA60RGBW, .zigbee_models_count=sizeof(kModels_SMARTZBA60RGBW)/sizeof(kModels_SMARTZBA60RGBW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SMARTZBA60RGBW", .vendor="Ledvance",
    .meta=nullptr, .exposes=kExposesLedvanceColorCTLight, .exposes_count=kExposesLedvanceColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLedvanceColorCTLight, .from_zigbee_count=kFzLedvanceColorCTLightCount,
    .to_zigbee=kTzLedvanceColorCTLight, .to_zigbee_count=kTzLedvanceColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsLedvanceColorCTLight,
    .bindings_count           = kBindingsLedvanceColorCTLightCount,
};

}  // namespace zhc::devices::ledvance
