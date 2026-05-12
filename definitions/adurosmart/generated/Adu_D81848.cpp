// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Adurosmart 81848 — hand-rewritten to use Adu_OnOffEM bundle.
// ERIA smart plug (with power measurements).
// z2m extend: fz.on_off + fz.electrical_measurement (no metering cluster — see ADUROSMART_PARITY.md).
// z2m-source: adurosmart.ts #81848.
#include "definitions/_generic/_shared.hpp"
#include "definitions/adurosmart/_shared.hpp"

namespace zhc::devices::adurosmart {
namespace {

constexpr const char* kModels_D81848[] = { "AD-SmartPlug3001" };

}  // namespace

extern const PreparedDefinition kDef_D81848{
    .zigbee_models=kModels_D81848,
    .zigbee_models_count=sizeof(kModels_D81848)/sizeof(kModels_D81848[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="81848", .vendor="Adurosmart",
    .meta=nullptr,
    .exposes=kExposesAduOnOffEM, .exposes_count=kExposesAduOnOffEMCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAduOnOffEM, .from_zigbee_count=kFzAduOnOffEMCount,
    .to_zigbee=kTzAduOnOffEM, .to_zigbee_count=kTzAduOnOffEMCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAduOnOffEM, .bindings_count=kBindingsAduOnOffEMCount,
};

}  // namespace zhc::devices::adurosmart
