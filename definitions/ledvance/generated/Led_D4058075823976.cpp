// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ledvance 4058075823976 — auto-generated.
// LED Tube T8 EM Connected 1200 mm 16W 840
// Bundle: OnOff (ON/OFF only).
// z2m-source: ledvance.ts #4058075823976.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ledvance/_shared.hpp"

namespace zhc::devices::ledvance {
namespace {

constexpr const char* kModels_D4058075823976[] = { "TUBE_T8_CON_1200_16W_840ZBV" };

}  // namespace

extern const PreparedDefinition kDef_D4058075823976{
    .zigbee_models=kModels_D4058075823976, .zigbee_models_count=sizeof(kModels_D4058075823976)/sizeof(kModels_D4058075823976[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4058075823976", .vendor="Ledvance",
    .meta=nullptr, .exposes=kExposesLedvanceOnOff, .exposes_count=kExposesLedvanceOnOffCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLedvanceOnOff, .from_zigbee_count=kFzLedvanceOnOffCount,
    .to_zigbee=kTzLedvanceOnOff, .to_zigbee_count=kTzLedvanceOnOffCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsLedvanceOnOff,
    .bindings_count           = kBindingsLedvanceOnOffCount,
};

}  // namespace zhc::devices::ledvance
