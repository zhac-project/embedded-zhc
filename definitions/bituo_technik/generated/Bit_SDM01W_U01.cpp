// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: BituoTechnik SDM01W-U01 — auto-generated.
// Smart energy monitor for 3P+N system
// z2m-source: bituo_technik.ts #SDM01W-U01.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bituo_technik {
namespace {
const FzConverter* const kFz_SDM01W_U01[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_SDM01W_U01[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_SDM01W_U01[] = { "SDM01W", "SDM01-3Z1" };

}  // namespace


// Hand-extended 2026-04-28: full 3-phase exposes per z2m TS extend block
// (m.electricityMeter threePhase + producedEnergy + acFrequency + powerFactor)
// plus bituo_fz.total_power and bituo_fz.phase_energy custom fields.
// NOTE: total_power*, energy_phase_*, produced_energy_phase_*, ac_frequency,
// power_factor, power_reactive*, power_apparent*, *_phase_b, *_phase_c are
// catalogued (Access::State) but NOT yet decoded by kFzMetering /
// kFzElectricalMeasurement — they need vendor-specific fz support
// (currentTier{1,2,3}SummDelivered/Received, totalActive/Reactive/ApparentPower,
// acFrequency 0x0300, powerFactor 0x0510). See BITUO_TECHNIK_PARITY.md.
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
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
    {"energy_phase_a", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"energy_phase_b", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"energy_phase_c", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"produced_energy_phase_a", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"produced_energy_phase_b", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"produced_energy_phase_c", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0702},
    {1, 0x0B04},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_SDM01W_U01{
    .zigbee_models=kModels_SDM01W_U01, .zigbee_models_count=sizeof(kModels_SDM01W_U01)/sizeof(kModels_SDM01W_U01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SDM01W-U01", .vendor="BituoTechnik",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SDM01W_U01, .from_zigbee_count=sizeof(kFz_SDM01W_U01)/sizeof(kFz_SDM01W_U01[0]),
    .to_zigbee=kTz_SDM01W_U01, .to_zigbee_count=sizeof(kTz_SDM01W_U01)/sizeof(kTz_SDM01W_U01[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::bituo_technik
