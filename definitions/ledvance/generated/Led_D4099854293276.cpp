// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ledvance 4099854293276 — auto-generated.
// SMART+ Compact outdoor plug EU with energy meter
// Bundle: OnOffEM (ON/OFF + electricity meter).
// z2m-source: ledvance.ts #4099854293276.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ledvance/_shared.hpp"

namespace zhc::devices::ledvance {
namespace {

constexpr const char* kModels_D4099854293276[] = { "PLUG COMPACT OUTDOOR EU EM T", "PLUG COMPACT EU EM T" };

}  // namespace

extern const PreparedDefinition kDef_D4099854293276{
    .zigbee_models=kModels_D4099854293276, .zigbee_models_count=sizeof(kModels_D4099854293276)/sizeof(kModels_D4099854293276[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4099854293276", .vendor="Ledvance",
    .meta=nullptr, .exposes=kExposesLedvanceOnOffEM, .exposes_count=kExposesLedvanceOnOffEMCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLedvanceOnOffEM, .from_zigbee_count=kFzLedvanceOnOffEMCount,
    .to_zigbee=kTzLedvanceOnOffEM, .to_zigbee_count=kTzLedvanceOnOffEMCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsLedvanceOnOffEM,
    .bindings_count           = kBindingsLedvanceOnOffEMCount,
};

}  // namespace zhc::devices::ledvance
