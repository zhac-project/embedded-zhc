// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ledvance 4058075729346 — auto-generated.
// SMART+ Gardenpole 5P Multicolor
// Bundle: ColorCTLight (RGB + tunable white).
// z2m-source: ledvance.ts #4058075729346.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ledvance/_shared.hpp"

namespace zhc::devices::ledvance {
namespace {

constexpr const char* kModels_D4058075729346[] = { "GARDENPOLE RGBW T" };

}  // namespace

extern const PreparedDefinition kDef_D4058075729346{
    .zigbee_models=kModels_D4058075729346, .zigbee_models_count=sizeof(kModels_D4058075729346)/sizeof(kModels_D4058075729346[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4058075729346", .vendor="Ledvance",
    .meta=nullptr, .exposes=kExposesLedvanceColorCTLight, .exposes_count=kExposesLedvanceColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLedvanceColorCTLight, .from_zigbee_count=kFzLedvanceColorCTLightCount,
    .to_zigbee=kTzLedvanceColorCTLight, .to_zigbee_count=kTzLedvanceColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsLedvanceColorCTLight,
    .bindings_count           = kBindingsLedvanceColorCTLightCount,
};

}  // namespace zhc::devices::ledvance
