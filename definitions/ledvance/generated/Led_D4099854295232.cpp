// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ledvance 4099854295232 — auto-generated.
// SMART+ Plug indoor EU with energy meter 
// Bundle: OnOffEM (ON/OFF + electricity meter).
// z2m-source: ledvance.ts #4099854295232.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ledvance/_shared.hpp"

namespace zhc::devices::ledvance {
namespace {

constexpr const char* kModels_D4099854295232[] = { "PLUG EU EM T", "PLUG EU EM T, black" };

constexpr ::zhc::WhiteLabel kWhiteLabels_D4099854295232[] = {
    { "Ledvance", "4099854295256" },  // SMART+ Plug indoor EU with energy meter (Black) — fingerprint "PLUG EU EM T, black"
};

}  // namespace

extern const PreparedDefinition kDef_D4099854295232{
    .zigbee_models=kModels_D4099854295232, .zigbee_models_count=sizeof(kModels_D4099854295232)/sizeof(kModels_D4099854295232[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4099854295232", .vendor="Ledvance",
    .meta=nullptr, .exposes=kExposesLedvanceOnOffEM, .exposes_count=kExposesLedvanceOnOffEMCount,
    .white_labels=kWhiteLabels_D4099854295232, .white_labels_count=sizeof(kWhiteLabels_D4099854295232)/sizeof(kWhiteLabels_D4099854295232[0]),
    .from_zigbee=kFzLedvanceOnOffEM, .from_zigbee_count=kFzLedvanceOnOffEMCount,
    .to_zigbee=kTzLedvanceOnOffEM, .to_zigbee_count=kTzLedvanceOnOffEMCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsLedvanceOnOffEM,
    .bindings_count           = kBindingsLedvanceOnOffEMCount,
};

}  // namespace zhc::devices::ledvance
