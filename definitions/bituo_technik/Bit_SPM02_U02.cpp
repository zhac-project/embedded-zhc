// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: uses shared bituo_technik converters (full metering/electrical channel decode).
// Tier 1: BituoTechnik SPM02-U02 — auto-generated.
// Smart energy monitor for 3P+N system
// z2m-source: bituo_technik.ts #SPM02-U02.
#include "definitions/_generic/_shared.hpp"
#include "definitions/bituo_technik/_shared.hpp"

namespace zhc::devices::bituo_technik {
namespace {
const FzConverter* const kFz_SPM02_U02[] = {
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::bituo_technik::kFzBituoMeteringExtras,
    &::zhc::bituo_technik::kFzBituoElectricalMeasurementExtras,
};

constexpr const char* kModels_SPM02_U02[] = { "SPM02" };

}  // namespace


// Hand-extended 2026-04-28: 3P+N exposes per z2m TS extend
// (m.electricityMeter threePhase + producedEnergy + acFrequency + powerFactor)
// No on/off in U02 SKU.
// Full channel set now decoded by kFzBituoMeteringExtras +
// kFzBituoElectricalMeasurementExtras (wired below): produced_energy,
// ac_frequency, power_factor, power_reactive/power_apparent, total_power*,
// per-phase b/c voltage/current/power.
constexpr Expose kAutoExposes[] = {
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"produced_energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_phase_b", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_phase_c", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_reactive", ExposeType::Numeric, Access::State, "VAR", nullptr, nullptr, 0},
    {"power_reactive_phase_b", ExposeType::Numeric, Access::State, "VAR", nullptr, nullptr, 0},
    {"power_reactive_phase_c", ExposeType::Numeric, Access::State, "VAR", nullptr, nullptr, 0},
    {"power_apparent", ExposeType::Numeric, Access::State, "VA", nullptr, nullptr, 0},
    {"power_apparent_phase_b", ExposeType::Numeric, Access::State, "VA", nullptr, nullptr, 0},
    {"power_apparent_phase_c", ExposeType::Numeric, Access::State, "VA", nullptr, nullptr, 0},
    {"power_factor", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"power_factor_phase_b", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"power_factor_phase_c", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"voltage_phase_b", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"voltage_phase_c", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_phase_b", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_phase_c", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"ac_frequency", ExposeType::Numeric, Access::State, "Hz", nullptr, nullptr, 0},
    {"total_power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"total_power_reactive", ExposeType::Numeric, Access::State, "VAR", nullptr, nullptr, 0},
    {"total_power_apparent", ExposeType::Numeric, Access::State, "VA", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0702},
    {1, 0x0B04},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_SPM02_U02{
    .zigbee_models=kModels_SPM02_U02, .zigbee_models_count=sizeof(kModels_SPM02_U02)/sizeof(kModels_SPM02_U02[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SPM02-U02", .vendor="BituoTechnik",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SPM02_U02, .from_zigbee_count=sizeof(kFz_SPM02_U02)/sizeof(kFz_SPM02_U02[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::bituo_technik
