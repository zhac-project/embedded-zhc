// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ledvance GPDRPLOP401100CE — auto-generated.
// Panel TW LED 625 UGR19
// Bundle: ColorCTLight (RGB + tunable white).
// z2m-source: ledvance.ts #GPDRPLOP401100CE.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ledvance/_shared.hpp"

namespace zhc::devices::ledvance {
namespace {

constexpr const char* kModels_GPDRPLOP401100CE[] = { "Panel TW 620 UGR19" };

}  // namespace

extern const PreparedDefinition kDef_GPDRPLOP401100CE{
    .zigbee_models=kModels_GPDRPLOP401100CE, .zigbee_models_count=sizeof(kModels_GPDRPLOP401100CE)/sizeof(kModels_GPDRPLOP401100CE[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GPDRPLOP401100CE", .vendor="Ledvance",
    .meta=nullptr, .exposes=kExposesLedvanceColorCTLight, .exposes_count=kExposesLedvanceColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLedvanceColorCTLight, .from_zigbee_count=kFzLedvanceColorCTLightCount,
    .to_zigbee=kTzLedvanceColorCTLight, .to_zigbee_count=kTzLedvanceColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsLedvanceColorCTLight,
    .bindings_count           = kBindingsLedvanceColorCTLightCount,
};

}  // namespace zhc::devices::ledvance
