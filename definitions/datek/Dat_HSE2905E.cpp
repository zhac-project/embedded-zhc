// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Datek HSE2905E — hand-upgraded for full z2m parity.
// Datek Eva AMS HAN power-meter sensor.
// z2m stacks two m.electricityMeter() bundles:
//   metering  (producedEnergy:true)        → energy + produced_energy + power
//   electrical(threePhase:true, power:false)
//             → voltage/current phase A + phase B/C
//   plus m.temperature().
// NB: electricityMeter() defaults acFrequency:false / powerFactor:false and
// this device's electrical bundle does NOT override them, so z2m exposes
// neither ac_frequency nor power_factor — we intentionally do not either.
// The generic kFzMetering (energy/power) + kFzElectricalMeasurement
// (power/voltage/current) cover only the phase-A core; the Datek
// kFzMeteringExtras + kFzElectricalMeasurementExtras add produced_energy and
// phase B/C voltage/current ALONGSIDE the generics (mirrors
// definitions/bituo_technik). All raw pass-through; runtime scales downstream.
// z2m-source: datek.ts #HSE2905E.
#include "definitions/_generic/_shared.hpp"
#include "definitions/datek/_shared.hpp"

namespace zhc::devices::datek {
namespace {
const FzConverter* const kFz_HSE2905E[] = {
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzTemperature,
    &::zhc::datek::kFzMeteringExtras,
    &::zhc::datek::kFzElectricalMeasurementExtras,
};

constexpr const char* kModels_HSE2905E[] = { "Meter Reader" };

}  // namespace


// --- hand-curated exposes/bindings (z2m parity) ---
constexpr Expose kAutoExposes[] = {
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"produced_energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"voltage_phase_b", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"voltage_phase_c", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_phase_b", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_phase_c", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0702},  // seMetering
    {1, 0x0B04},  // haElectricalMeasurement
    {1, 0x0402},  // msTemperatureMeasurement
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_HSE2905E{
    .zigbee_models=kModels_HSE2905E, .zigbee_models_count=sizeof(kModels_HSE2905E)/sizeof(kModels_HSE2905E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HSE2905E", .vendor="Datek",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HSE2905E, .from_zigbee_count=sizeof(kFz_HSE2905E)/sizeof(kFz_HSE2905E[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::datek
