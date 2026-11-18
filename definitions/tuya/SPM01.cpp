// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya SPM01 smart energy monitor for 1P+N systems (`_TZE200_bcusnqt8`).
// Was TZE200_sleep.cpp, a hand def that called this fingerprint a bed
// presence monitor (occupancy / heart_rate / breath_rate); upstream has
// only ever listed it as SPM01 (R10 triage, PRE-EXISTING).
// DP1 / DP2 energy / produced_energy /100; DP6 phaseVariant4 (be16 voltage
// /10, 24-bit current /1000, 24-bit power); DP9 circuitBreakerFaults1.
// DP6 rides phase_variant2_with_phase: the same 8-byte layout, and its
// negative-power correction only applies at >= 0x800000 W, which a 63 A
// single-phase meter never reaches.
// z2m-source: tuya.ts #SPM01.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

// z2m `circuitBreakerFaultList` by bit position; bit 14 "magnetism" is
// published as "negative_power" by circuitBreakerFaults1.
constexpr const char* kFaultNames[] = {
    "short_circuit", "surge", "overload", "leakage_current",
    "temperature", "fire", "high_power", "self_test",
    "over_current", "unbalance", "over_voltage", "under_voltage",
    "miss_phase", "outage", "negative_power", "credit",
    "no_balance",
};
constexpr ::zhc::tuya::TuyaFaultTable kFaultTable{
    kFaultNames, sizeof(kFaultNames)/sizeof(kFaultNames[0]) };

struct cfg_spm01 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "energy", 100),
        ::zhc::tuya::dp::numeric(2, "produced_energy", 100),
        ::zhc::tuya::dp::phase_variant2_with_phase(6, &::zhc::tuya::kTuyaPhaseKeysPlain),
        ::zhc::tuya::dp::fault_bitmap(9, "faults", &kFaultTable),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg_spm01>;

constexpr Expose kExposes[] = {
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "V", nullptr, nullptr, 0 },
    { "power",   ExposeType::Numeric, ::zhc::Access::State, "W", nullptr, nullptr, 0 },
    { "current", ExposeType::Numeric, ::zhc::Access::State, "A", nullptr, nullptr, 0 },
    { "energy",  ExposeType::Numeric, ::zhc::Access::State, "kWh", "Total forward active energy", nullptr, 0 },
    { "produced_energy", ExposeType::Numeric, ::zhc::Access::State, "kWh", "Total reverse active energy", nullptr, 0 },
    { "faults", ExposeType::String, ::zhc::Access::State, nullptr,
      "Active faults, comma separated; empty when none", nullptr, 0, ExposeCategory::Diagnostic },
};

constexpr const char* kModels[] = { "TS0601" };
constexpr const char* kManus[]  = { "_TZE200_bcusnqt8" };

}  // namespace

extern const PreparedDefinition kDef_SPM01{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="SPM01", .vendor="Tuya",
    .meta=nullptr,
    .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::tuya
