// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ledvance 4058075729285 — auto-generated.
// SMART+ Plug UK
// Bundle: OnOff (ON/OFF only).
// z2m-source: ledvance.ts #4058075729285.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ledvance/_shared.hpp"

namespace zhc::devices::ledvance {
namespace {

constexpr const char* kModels_D4058075729285[] = { "PLUG UK T", "PLUG EU T" };

constexpr ::zhc::WhiteLabel kWhiteLabels_D4058075729285[] = {
    { "LEDVANCE", "4058075729261" },  // SMART+ Plug EU — fingerprint "PLUG EU T"
};

}  // namespace

extern const PreparedDefinition kDef_D4058075729285{
    .zigbee_models=kModels_D4058075729285, .zigbee_models_count=sizeof(kModels_D4058075729285)/sizeof(kModels_D4058075729285[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4058075729285", .vendor="Ledvance",
    .meta=nullptr, .exposes=kExposesLedvanceOnOff, .exposes_count=kExposesLedvanceOnOffCount,
    .white_labels=kWhiteLabels_D4058075729285, .white_labels_count=sizeof(kWhiteLabels_D4058075729285)/sizeof(kWhiteLabels_D4058075729285[0]),
    .from_zigbee=kFzLedvanceOnOff, .from_zigbee_count=kFzLedvanceOnOffCount,
    .to_zigbee=kTzLedvanceOnOff, .to_zigbee_count=kTzLedvanceOnOffCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsLedvanceOnOff,
    .bindings_count           = kBindingsLedvanceOnOffCount,
};

}  // namespace zhc::devices::ledvance
