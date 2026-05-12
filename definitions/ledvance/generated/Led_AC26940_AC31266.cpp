// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ledvance AC26940/AC31266 — auto-generated.
// Smart Zigbee outdoor plug
// Bundle: OnOff (ON/OFF only).
// z2m-source: ledvance.ts #AC26940/AC31266.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ledvance/_shared.hpp"

namespace zhc::devices::ledvance {
namespace {

constexpr const char* kModels_AC26940_AC31266[] = { "Outdoor Plug" };

}  // namespace

extern const PreparedDefinition kDef_AC26940_AC31266{
    .zigbee_models=kModels_AC26940_AC31266, .zigbee_models_count=sizeof(kModels_AC26940_AC31266)/sizeof(kModels_AC26940_AC31266[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AC26940/AC31266", .vendor="Ledvance",
    .meta=nullptr, .exposes=kExposesLedvanceOnOff, .exposes_count=kExposesLedvanceOnOffCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLedvanceOnOff, .from_zigbee_count=kFzLedvanceOnOffCount,
    .to_zigbee=kTzLedvanceOnOff, .to_zigbee_count=kTzLedvanceOnOffCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsLedvanceOnOff,
    .bindings_count           = kBindingsLedvanceOnOffCount,
};

}  // namespace zhc::devices::ledvance
