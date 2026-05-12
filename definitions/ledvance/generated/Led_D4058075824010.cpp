// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ledvance 4058075824010 — auto-generated.
// LED Tube T8 EM Connected P 1500mm 24W 840
// Bundle: DimEM (dimmable + electricity meter).
// z2m-source: ledvance.ts #4058075824010.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ledvance/_shared.hpp"

namespace zhc::devices::ledvance {
namespace {

constexpr const char* kModels_D4058075824010[] = { "TUBE_T8_CON_1500_24W_840ZBV" };

}  // namespace

extern const PreparedDefinition kDef_D4058075824010{
    .zigbee_models=kModels_D4058075824010, .zigbee_models_count=sizeof(kModels_D4058075824010)/sizeof(kModels_D4058075824010[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4058075824010", .vendor="Ledvance",
    .meta=nullptr, .exposes=kExposesLedvanceDimEM, .exposes_count=kExposesLedvanceDimEMCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLedvanceDimEM, .from_zigbee_count=kFzLedvanceDimEMCount,
    .to_zigbee=kTzLedvanceDimEM, .to_zigbee_count=kTzLedvanceDimEMCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsLedvanceDimEM,
    .bindings_count           = kBindingsLedvanceDimEMCount,
};

}  // namespace zhc::devices::ledvance
