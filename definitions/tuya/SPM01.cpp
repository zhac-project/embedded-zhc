// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya SPM01 smart energy monitor for 1P+N systems (`_TZE200_bcusnqt8`),
// plus SPM01V2.5 (`iwn0gpzz`) and the three-phase SPM02 (`ves1ycwx`) below.
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

// ── SPM01V2.5 / SPM02 (z2m v26.133.0, #13599) ─────────────────────────
// Thresholds are read-only (multi-key RMW writes not ported), the polling /
// DP query on announce is not ported, and DP18 id 8 is emitted as-is (no
// sticky "keep the user's < 30 s interval" rule). energy_reset follows
// Nou_D4Z: write false (what upstream's `reset` sends for "RESET").
namespace {
// spm01v25Thresholds: ids 1/3/4 alarm + value, 7/9 flag-only, 8 value-only.
constexpr ::zhc::tuya::TuyaThresholdDef kV25Defs[] = {
    { 1, "over_current_alarm",   "over_current_threshold"  },
    { 3, "over_voltage_alarm",   "over_voltage_threshold"  },
    { 4, "under_voltage_alarm",  "under_voltage_threshold" },
    { 7, "reverse_energy_alarm", nullptr                   },
    { 8, nullptr,                "data_report_duration"    },
    { 9, "device_locating",      nullptr                   },
};
constexpr ::zhc::tuya::TuyaThresholdTable kV25{ kV25Defs, sizeof(kV25Defs)/sizeof(kV25Defs[0]) };
// threshold_7 (alarmSet1B): only the exposed high-power record.
constexpr ::zhc::tuya::TuyaThresholdDef kV25Set1Defs[] = { { 7, "high_power_alarm", "high_power_threshold" } };
constexpr ::zhc::tuya::TuyaThresholdTable kV25Set1{ kV25Set1Defs, 1 };
struct cfg_spm01v25 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "energy", 100),
        ::zhc::tuya::dp::phase_variant2_with_phase(6, &::zhc::tuya::kTuyaPhaseKeysPlain),
        ::zhc::tuya::dp::fault_bitmap(9, "faults", &kFaultTable),
        ::zhc::tuya::dp::binary(16, "energy_reset"),
        ::zhc::tuya::dp::thresholds(17, &kV25Set1),
        ::zhc::tuya::dp::thresholds(18, &kV25),
        ::zhc::tuya::dp::numeric(23, "produced_energy", 100),
        ::zhc::tuya::dp::numeric(29, "power", 1),
        ::zhc::tuya::dp::numeric(32, "ac_frequency", 100),
        ::zhc::tuya::dp::numeric(50, "power_factor", 1),
        ::zhc::tuya::dp::numeric(102, "voltage", 10),
        ::zhc::tuya::dp::numeric(103, "current", 1000),
        ::zhc::tuya::dp::numeric(104, "power", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX25 = ::zhc::tuya::factory::TuyaRw<cfg_spm01v25>;
constexpr Expose kExposesV25[] = {
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "V", nullptr, nullptr, 0 },
    { "power",   ExposeType::Numeric, ::zhc::Access::State, "W", nullptr, nullptr, 0 },
    { "current", ExposeType::Numeric, ::zhc::Access::State, "A", nullptr, nullptr, 0 },
    { "energy",  ExposeType::Numeric, ::zhc::Access::State, "kWh", "Total forward active energy", nullptr, 0 },
    { "produced_energy", ExposeType::Numeric, ::zhc::Access::State, "kWh", "Total reverse active energy", nullptr, 0 },
    { "power_factor", ExposeType::Numeric, ::zhc::Access::State, "%", nullptr, nullptr, 0 },
    { "ac_frequency", ExposeType::Numeric, ::zhc::Access::State, "Hz", nullptr, nullptr, 0 },
    { "data_report_duration", ExposeType::Numeric, ::zhc::Access::State, "s", "Data report duration", nullptr, 0 },
    { "over_current_alarm",      ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "over_current_threshold",  ExposeType::Numeric, ::zhc::Access::State, "A", nullptr, nullptr, 0, ExposeCategory::Config },
    { "over_voltage_alarm",      ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "over_voltage_threshold",  ExposeType::Numeric, ::zhc::Access::State, "V", nullptr, nullptr, 0, ExposeCategory::Config },
    { "under_voltage_alarm",     ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "under_voltage_threshold", ExposeType::Numeric, ::zhc::Access::State, "V", nullptr, nullptr, 0, ExposeCategory::Config },
    { "reverse_energy_alarm",    ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "device_locating",         ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "high_power_threshold", ExposeType::Numeric, ::zhc::Access::State, "kW", nullptr, nullptr, 0, ExposeCategory::Diagnostic },
    { "high_power_alarm",     ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Diagnostic },
    { "faults", ExposeType::String, ::zhc::Access::State, nullptr,
      "Active faults, comma separated; empty when none", nullptr, 0, ExposeCategory::Diagnostic },
    { "energy_reset", ExposeType::Binary, ::zhc::Access::Set, nullptr,
      "Write false to reset the energy counters (the wire value upstream sends for RESET)", nullptr, 0, ExposeCategory::Config },
};
constexpr const char* kManusV25[] = { "_TZE200_iwn0gpzz", "_TZE204_iwn0gpzz", "_TZE284_iwn0gpzz" };

// spm02Thresholds: ids 1-4 alarm + value, 5/7/9 flag-only.
constexpr ::zhc::tuya::TuyaThresholdDef kSpm02Defs[] = {
    { 1, "over_current_alarm",    "over_current_threshold"    },
    { 2, "unbalanced_load_alarm", "unbalanced_load_threshold" },
    { 3, "over_voltage_alarm",    "over_voltage_threshold"    },
    { 4, "under_voltage_alarm",   "under_voltage_threshold"   },
    { 5, "phase_loss_alarm",      nullptr                     },
    { 7, "reverse_energy_alarm",  nullptr                     },
    { 9, "device_locating",       nullptr                     },
};
constexpr ::zhc::tuya::TuyaThresholdTable kSpm02{ kSpm02Defs, sizeof(kSpm02Defs)/sizeof(kSpm02Defs[0]) };
constexpr ::zhc::tuya::TuyaPhaseKeys kPhaseX{ "voltage_X", "current_X", "power_X" };
constexpr ::zhc::tuya::TuyaPhaseKeys kPhaseY{ "voltage_Y", "current_Y", "power_Y" };
constexpr ::zhc::tuya::TuyaPhaseKeys kPhaseZ{ "voltage_Z", "current_Z", "power_Z" };
struct cfg_spm02 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "energy", 100),
        ::zhc::tuya::dp::numeric(2, "produced_energy", 100),
        ::zhc::tuya::dp::phase_variant2_with_phase(6, &kPhaseX),
        ::zhc::tuya::dp::phase_variant2_with_phase(7, &kPhaseY),
        ::zhc::tuya::dp::phase_variant2_with_phase(8, &kPhaseZ),
        ::zhc::tuya::dp::fault_bitmap(9, "faults", &kFaultTable),
        ::zhc::tuya::dp::binary(12, "energy_reset"),
        ::zhc::tuya::dp::thresholds(18, &kSpm02),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX02 = ::zhc::tuya::factory::TuyaRw<cfg_spm02>;
constexpr Expose kExposes02[] = {
    { "voltage_X", ExposeType::Numeric, ::zhc::Access::State, "V", "Measured electrical potential value (phase X)", nullptr, 0 },
    { "voltage_Y", ExposeType::Numeric, ::zhc::Access::State, "V", "Measured electrical potential value (phase Y)", nullptr, 0 },
    { "voltage_Z", ExposeType::Numeric, ::zhc::Access::State, "V", "Measured electrical potential value (phase Z)", nullptr, 0 },
    { "power_X", ExposeType::Numeric, ::zhc::Access::State, "W", "Instantaneous measured power (phase X)", nullptr, 0 },
    { "power_Y", ExposeType::Numeric, ::zhc::Access::State, "W", "Instantaneous measured power (phase Y)", nullptr, 0 },
    { "power_Z", ExposeType::Numeric, ::zhc::Access::State, "W", "Instantaneous measured power (phase Z)", nullptr, 0 },
    { "current_X", ExposeType::Numeric, ::zhc::Access::State, "A", "Instantaneous measured electrical current (phase X)", nullptr, 0 },
    { "current_Y", ExposeType::Numeric, ::zhc::Access::State, "A", "Instantaneous measured electrical current (phase Y)", nullptr, 0 },
    { "current_Z", ExposeType::Numeric, ::zhc::Access::State, "A", "Instantaneous measured electrical current (phase Z)", nullptr, 0 },
    { "energy",  ExposeType::Numeric, ::zhc::Access::State, "kWh", "Total forward active energy", nullptr, 0 },
    { "produced_energy", ExposeType::Numeric, ::zhc::Access::State, "kWh", "Total reverse active energy", nullptr, 0 },
    { "energy_reset", ExposeType::Binary, ::zhc::Access::Set, nullptr,
      "Write false to reset the energy counters (the wire value upstream sends for RESET)", nullptr, 0, ExposeCategory::Config },
    { "over_current_alarm",        ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "over_current_threshold",    ExposeType::Numeric, ::zhc::Access::State, "A", nullptr, nullptr, 0, ExposeCategory::Config },
    { "unbalanced_load_alarm",     ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "unbalanced_load_threshold", ExposeType::Numeric, ::zhc::Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Config },
    { "over_voltage_alarm",        ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "over_voltage_threshold",    ExposeType::Numeric, ::zhc::Access::State, "V", nullptr, nullptr, 0, ExposeCategory::Config },
    { "under_voltage_alarm",       ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "under_voltage_threshold",   ExposeType::Numeric, ::zhc::Access::State, "V", nullptr, nullptr, 0, ExposeCategory::Config },
    { "phase_loss_alarm",          ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "reverse_energy_alarm",      ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "device_locating",           ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "faults", ExposeType::String, ::zhc::Access::State, nullptr,
      "Active faults, comma separated; empty when none", nullptr, 0, ExposeCategory::Diagnostic },
};
constexpr const char* kManus02[] = { "_TZE200_ves1ycwx", "_TZE204_ves1ycwx", "_TZE284_ves1ycwx" };
}  // namespace
extern const PreparedDefinition kDef_SPM01V25{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManusV25, .manufacturer_names_count=sizeof(kManusV25)/sizeof(kManusV25[0]),
    .model="SPM01V2.5", .vendor="Tuya",
    .meta=nullptr,
    .exposes=kExposesV25, .exposes_count=sizeof(kExposesV25)/sizeof(kExposesV25[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX25::fz_list, .from_zigbee_count=FX25::fz_count,
    .to_zigbee=FX25::tz_list, .to_zigbee_count=FX25::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };
extern const PreparedDefinition kDef_SPM02{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus02, .manufacturer_names_count=sizeof(kManus02)/sizeof(kManus02[0]),
    .model="SPM02", .vendor="Tuya",
    .meta=nullptr,
    .exposes=kExposes02, .exposes_count=sizeof(kExposes02)/sizeof(kExposes02[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX02::fz_list, .from_zigbee_count=FX02::fz_count,
    .to_zigbee=FX02::tz_list, .to_zigbee_count=FX02::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::tuya
