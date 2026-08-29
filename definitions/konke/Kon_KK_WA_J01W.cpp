// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Konke KK-WA-J01W — IAS water leak detector (typed).
// GRADUATED from generated/ for the z2m v26.101.0 split.
//
// Upstream SPLIT this entry in v26.101.0: `3AFE12010402102A` moved out to a
// new `KK-WA-J01W-2020` definition because that firmware revision needs a
// different battery voltage-to-percentage curve (2500-3300 mV instead of the
// generic one). The token diff shows this only as one removed zigbeeModel on
// this entry, and the sibling never surfaced in the new-device bucket because
// its fingerprint was already "covered" — by this very definition. Deleting
// the model without adding the sibling would have dropped the device entirely.
// See Kon_KK_WA_J01W_2020.cpp.
//
// z2m-source: konke.ts #KK-WA-J01W (fz.ias_water_leak_alarm_1).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::konke {
namespace {
const FzConverter* const kFz_KK_WA_J01W[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasWaterLeakAlarm,
};

constexpr const char* kModels_KK_WA_J01W[] = { "3AFE21100402102A", "3AFE22010402102A" };

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"water_leak",  ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone
};

}  // namespace

extern const PreparedDefinition kDef_KK_WA_J01W{
    .zigbee_models=kModels_KK_WA_J01W, .zigbee_models_count=sizeof(kModels_KK_WA_J01W)/sizeof(kModels_KK_WA_J01W[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="KK-WA-J01W", .vendor="Konke",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_KK_WA_J01W, .from_zigbee_count=sizeof(kFz_KK_WA_J01W)/sizeof(kFz_KK_WA_J01W[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::konke
