// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Adurosmart 81998 — hand-rewritten to use Adu_OnOffEM bundle.
// ERIA built-in on/off relay (with power measurements).
// z2m extend: m.onOff() + m.electricityMeter({cluster: "electrical"}).
// z2m-source: adurosmart.ts #81998.
// Fingerprint: modelID="ONOFF_METER_RELAY", manufacturerName="AduroSmart ERIA".
#include "definitions/_generic/_shared.hpp"
#include "definitions/adurosmart/_shared.hpp"

namespace zhc::devices::adurosmart {
namespace {

constexpr const char* kModels_D81998[] = { "ONOFF_METER_RELAY" };
constexpr const char* kMfgNames_D81998[] = { "AduroSmart ERIA" };

}  // namespace

extern const PreparedDefinition kDef_D81998{
    .zigbee_models=kModels_D81998,
    .zigbee_models_count=sizeof(kModels_D81998)/sizeof(kModels_D81998[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kMfgNames_D81998,
    .manufacturer_names_count=sizeof(kMfgNames_D81998)/sizeof(kMfgNames_D81998[0]),
    .model="81998", .vendor="Adurosmart",
    .meta=nullptr,
    .exposes=kExposesAduOnOffEM, .exposes_count=kExposesAduOnOffEMCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAduOnOffEM, .from_zigbee_count=kFzAduOnOffEMCount,
    .to_zigbee=kTzAduOnOffEM, .to_zigbee_count=kTzAduOnOffEMCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAduOnOffEM, .bindings_count=kBindingsAduOnOffEMCount,
};

}  // namespace zhc::devices::adurosmart
