// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ledvance 4099854513718 — auto-generated.
// PL CMFT 600 ZB P 33W 840 U19
// Bundle: Dim (dimmable).
// z2m-source: ledvance.ts #4099854513718.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ledvance/_shared.hpp"

namespace zhc::devices::ledvance {
namespace {

constexpr const char* kModels_D4099854513718[] = { "DR_ZBD_NFC_P_45W_220-240V_1A2" };

}  // namespace

extern const PreparedDefinition kDef_D4099854513718{
    .zigbee_models=kModels_D4099854513718, .zigbee_models_count=sizeof(kModels_D4099854513718)/sizeof(kModels_D4099854513718[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4099854513718", .vendor="Ledvance",
    .meta=nullptr, .exposes=kExposesLedvanceDim, .exposes_count=kExposesLedvanceDimCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLedvanceDim, .from_zigbee_count=kFzLedvanceDimCount,
    .to_zigbee=kTzLedvanceDim, .to_zigbee_count=kTzLedvanceDimCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsLedvanceDim,
    .bindings_count           = kBindingsLedvanceDimCount,
};

}  // namespace zhc::devices::ledvance
