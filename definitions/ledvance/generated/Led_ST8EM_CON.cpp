// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ledvance ST8EM-CON — auto-generated.
// SubstiTUBE connected advanced ultra output
// Bundle: Dim (dimmable).
// z2m-source: ledvance.ts #ST8EM-CON.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ledvance/_shared.hpp"

namespace zhc::devices::ledvance {
namespace {

constexpr const char* kModels_ST8EM_CON[] = { "Connected Tube Value II", "TUBE_T8_CON_600_7.5W_840ZBV\u0000" };

}  // namespace

extern const PreparedDefinition kDef_ST8EM_CON{
    .zigbee_models=kModels_ST8EM_CON, .zigbee_models_count=sizeof(kModels_ST8EM_CON)/sizeof(kModels_ST8EM_CON[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ST8EM-CON", .vendor="Ledvance",
    .meta=nullptr, .exposes=kExposesLedvanceDim, .exposes_count=kExposesLedvanceDimCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLedvanceDim, .from_zigbee_count=kFzLedvanceDimCount,
    .to_zigbee=kTzLedvanceDim, .to_zigbee_count=kTzLedvanceDimCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsLedvanceDim,
    .bindings_count           = kBindingsLedvanceDimCount,
};

}  // namespace zhc::devices::ledvance
