// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ledvance AC33903 — auto-generated.
// SMART+ classic P 40 E14 tunable white
// Bundle: Light (tunable white (CT)).
// z2m-source: ledvance.ts #AC33903.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ledvance/_shared.hpp"

namespace zhc::devices::ledvance {
namespace {

constexpr const char* kModels_AC33903[] = { "P40S TW" };

}  // namespace

extern const PreparedDefinition kDef_AC33903{
    .zigbee_models=kModels_AC33903, .zigbee_models_count=sizeof(kModels_AC33903)/sizeof(kModels_AC33903[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AC33903", .vendor="Ledvance",
    .meta=nullptr, .exposes=kExposesLedvanceLight, .exposes_count=kExposesLedvanceLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLedvanceLight, .from_zigbee_count=kFzLedvanceLightCount,
    .to_zigbee=kTzLedvanceLight, .to_zigbee_count=kTzLedvanceLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsLedvanceLight,
    .bindings_count           = kBindingsLedvanceLightCount,
};

}  // namespace zhc::devices::ledvance
