// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nodon SEM-4-1-00 — hand-rewrite (added voltage/current).
// Energy monitoring sensor.
//
// z2m: `m.electricityMeter({acFrequency:true, powerFactor:true,
//        producedEnergy:true})`. The default electricityMeter under z2m
// emits energy/power; with the extra flags it adds ac_frequency,
// power_factor, energy_produced (clusters: seMetering 0x0702 +
// haElectricalMeasurement 0x0B04). The generic `kFzElectricalMeasurement`
// gives us power/voltage/current; ac_frequency, power_factor and
// energy_produced still need a per-vendor extender (TODO).
//
// z2m-source: nodon.ts #SEM-4-1-00.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nodon {
namespace {
const FzConverter* const kFz_SEM_4_1_00[] = {
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};

constexpr const char* kModels_SEM_4_1_00[] = { "SEM-4-1-00" };

}  // namespace


// Hand-aligned to z2m exposes (added voltage/current; ac_frequency,
// power_factor, energy_produced still TODO).
constexpr Expose kAutoExposes[] = {
    {"energy",  ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power",   ExposeType::Numeric, Access::State, "W",   nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V",   nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A",   nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0702},
    {1, 0x0B04},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_SEM_4_1_00{
    .zigbee_models=kModels_SEM_4_1_00, .zigbee_models_count=sizeof(kModels_SEM_4_1_00)/sizeof(kModels_SEM_4_1_00[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SEM-4-1-00", .vendor="Nodon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SEM_4_1_00, .from_zigbee_count=sizeof(kFz_SEM_4_1_00)/sizeof(kFz_SEM_4_1_00[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nodon
