// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ledvance 4058075729223 — auto-generated.
// SMART+ Filament Edison 52 E27 Amber dimmable
// Bundle: Dim (dimmable).
// z2m-source: ledvance.ts #4058075729223.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ledvance/_shared.hpp"

namespace zhc::devices::ledvance {
namespace {

constexpr const char* kModels_D4058075729223[] = { "EDISON60 FIL DIM T" };

}  // namespace

extern const PreparedDefinition kDef_D4058075729223{
    .zigbee_models=kModels_D4058075729223, .zigbee_models_count=sizeof(kModels_D4058075729223)/sizeof(kModels_D4058075729223[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4058075729223", .vendor="Ledvance",
    .meta=nullptr, .exposes=kExposesLedvanceDim, .exposes_count=kExposesLedvanceDimCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLedvanceDim, .from_zigbee_count=kFzLedvanceDimCount,
    .to_zigbee=kTzLedvanceDim, .to_zigbee_count=kTzLedvanceDimCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsLedvanceDim,
    .bindings_count           = kBindingsLedvanceDimCount,
};

}  // namespace zhc::devices::ledvance
