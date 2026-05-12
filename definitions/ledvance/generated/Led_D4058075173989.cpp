// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ledvance 4058075173989 — auto-generated.
// SMART+ indoor undercabinet light
// Bundle: Light (tunable white (CT)).
// z2m-source: ledvance.ts #4058075173989.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ledvance/_shared.hpp"

namespace zhc::devices::ledvance {
namespace {

constexpr const char* kModels_D4058075173989[] = { "Undercabinet TW Z3" };

}  // namespace

extern const PreparedDefinition kDef_D4058075173989{
    .zigbee_models=kModels_D4058075173989, .zigbee_models_count=sizeof(kModels_D4058075173989)/sizeof(kModels_D4058075173989[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4058075173989", .vendor="Ledvance",
    .meta=nullptr, .exposes=kExposesLedvanceLight, .exposes_count=kExposesLedvanceLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLedvanceLight, .from_zigbee_count=kFzLedvanceLightCount,
    .to_zigbee=kTzLedvanceLight, .to_zigbee_count=kTzLedvanceLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsLedvanceLight,
    .bindings_count           = kBindingsLedvanceLightCount,
};

}  // namespace zhc::devices::ledvance
