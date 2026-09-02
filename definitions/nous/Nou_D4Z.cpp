// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Nous D4Z three-phase DIN-rail energy meter
// (`_TZE204_loejka0i` / `_TZE284_loejka0i` / `_TZE200_loejka0i`).
//
// Graduated 2026-09-02 out of tuya/generated/ (two per-manufacturer copies)
// to pick up the `_TZE200_loejka0i` batch z2m v26.103.0 detects (#13070),
// and to wire the datapoints the generated copies never carried:
//   dp9   circuitBreakerFaults1 — fault bitmap, joined into one `faults`
//         string (z2m publishes a list; ZHC has no list Value)
//   dp16  energy_reset — write-only. Upstream's `reset` converter sends Bool
//         false for the "RESET" enum, so writing `false` here is the reset.
//   dp17  threshold_7 — RS-485 config + high-power alarm records
//   dp18  threshold_8 — over/under voltage, over-current, unbalanced load,
//         phase loss, negative power, reporting interval, locating
// Unlike the sibling D4Z-M this meter reports per-phase voltage / current /
// power on plain numeric datapoints 102-110 — no packed phase blobs (upstream
// comments dp6/7/8 out as duplicates).
//
// Same two upstream post-processing steps NOT reproduced as on the D4Z-M:
// `rs485_baud_rate` is the raw exponent (z2m publishes 1200 * 2^raw) and
// `data_reporting_interval` is the raw value (z2m halves it). Thresholds are
// read-only here for the reason recorded on the D4Z-M (read-modify-write
// over the whole record blob; 32 B of scratch is not enough).
//
// z2m-source: nous.ts #D4Z.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::nous {
namespace {

// z2m `circuitBreakerFaultList`, indexed by BIT POSITION; bit 14 is named
// "magnetism" upstream but published as "negative_power".
constexpr const char* kFaultNames[] = {
    "short_circuit", "surge", "overload", "leakage_current",
    "temperature", "fire", "high_power", "self_test",
    "over_current", "unbalance", "over_voltage", "under_voltage",
    "miss_phase", "outage", "negative_power", "credit",
    "no_balance",
};
constexpr ::zhc::tuya::TuyaFaultTable kFaultTable{
    kFaultNames, sizeof(kFaultNames)/sizeof(kFaultNames[0]) };

// dp17 — z2m `alarmSet1BThresholdDefinitions` (threshold_7).
constexpr ::zhc::tuya::TuyaThresholdDef kAlarmSet1Defs[] = {
    { 3, "rs485_baud_rate_enabled",   "rs485_baud_rate"    },
    { 4, "rs485_address_enabled",     "rs485_address"      },
    { 5, "rs485_data_format_enabled", "rs485_data_format"  },
    { 7, "high_power_alarm",          "high_power_threshold" },
};
constexpr ::zhc::tuya::TuyaThresholdTable kAlarmSet1{
    kAlarmSet1Defs, sizeof(kAlarmSet1Defs)/sizeof(kAlarmSet1Defs[0]) };

// dp18 — z2m `alarmSet2ThresholdDefinitions` (threshold_8). Ids 5 and 7 are
// flag-only.
constexpr ::zhc::tuya::TuyaThresholdDef kAlarmSet2Defs[] = {
    { 1, "over_current_alarm",           "over_current_threshold"     },
    { 2, "unbalanced_load_alarm",        "unbalanced_load_threshold"  },
    { 3, "over_voltage_alarm",           "over_voltage_threshold"     },
    { 4, "under_voltage_alarm",          "under_voltage_threshold"    },
    { 5, "phase_loss_alarm",             nullptr                      },
    { 7, "negative_active_power_alarm",  nullptr                      },
    { 8, "custom_data_reporting_interval", "data_reporting_interval"  },
    { 9, "device_locating",              "device_locating_threshold"  },
};
constexpr ::zhc::tuya::TuyaThresholdTable kAlarmSet2{
    kAlarmSet2Defs, sizeof(kAlarmSet2Defs)/sizeof(kAlarmSet2Defs[0]) };

struct cfg_d4z {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1,   "energy",            100),
        ::zhc::tuya::dp::numeric(2,   "produced_energy",   100),
        ::zhc::tuya::dp::fault_bitmap(9, "faults", &kFaultTable),
        ::zhc::tuya::dp::numeric(15,  "power_factor",      1),
        ::zhc::tuya::dp::binary(16,   "energy_reset"),
        ::zhc::tuya::dp::thresholds(17, &kAlarmSet1),
        ::zhc::tuya::dp::thresholds(18, &kAlarmSet2),
        ::zhc::tuya::dp::numeric(101, "ac_frequency",      100),
        ::zhc::tuya::dp::numeric(102, "voltage_a",         10),
        ::zhc::tuya::dp::numeric(103, "current_a",         1000),
        ::zhc::tuya::dp::numeric(104, "power_a",           1),
        ::zhc::tuya::dp::numeric(105, "voltage_b",         10),
        ::zhc::tuya::dp::numeric(106, "current_b",         1000),
        ::zhc::tuya::dp::numeric(107, "power_b",           1),
        ::zhc::tuya::dp::numeric(108, "voltage_c",         10),
        ::zhc::tuya::dp::numeric(109, "current_c",         1000),
        ::zhc::tuya::dp::numeric(110, "power_c",           1),
        ::zhc::tuya::dp::numeric(111, "power",             1),
        ::zhc::tuya::dp::numeric(112, "energy_a",          100),
        ::zhc::tuya::dp::numeric(113, "energy_produced_a", 100),
        ::zhc::tuya::dp::numeric(114, "energy_b",          100),
        ::zhc::tuya::dp::numeric(115, "energy_produced_b", 100),
        ::zhc::tuya::dp::numeric(116, "energy_c",          100),
        ::zhc::tuya::dp::numeric(117, "energy_produced_c", 100),
        ::zhc::tuya::dp::numeric(118, "power_factor_a",    1),
        ::zhc::tuya::dp::numeric(119, "power_factor_b",    1),
        ::zhc::tuya::dp::numeric(120, "power_factor_c",    1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXD4z = ::zhc::tuya::factory::TuyaRw<cfg_d4z>;

constexpr Expose kExposes_D4Z[] = {
    // Totals
    { "energy",          ExposeType::Numeric, ::zhc::Access::State, "kWh", "Total consumed energy", nullptr, 0 },
    { "produced_energy", ExposeType::Numeric, ::zhc::Access::State, "kWh", "Total produced energy", nullptr, 0 },
    { "power",           ExposeType::Numeric, ::zhc::Access::State, "W",   "Total active power",    nullptr, 0 },
    { "power_factor",    ExposeType::Numeric, ::zhc::Access::State, "%",   "Total power factor",    nullptr, 0 },
    { "ac_frequency",    ExposeType::Numeric, ::zhc::Access::State, "Hz",  nullptr, nullptr, 0 },
    // Per phase
    { "voltage_a", ExposeType::Numeric, ::zhc::Access::State, "V", "Phase A voltage", nullptr, 0 },
    { "voltage_b", ExposeType::Numeric, ::zhc::Access::State, "V", "Phase B voltage", nullptr, 0 },
    { "voltage_c", ExposeType::Numeric, ::zhc::Access::State, "V", "Phase C voltage", nullptr, 0 },
    { "current_a", ExposeType::Numeric, ::zhc::Access::State, "A", "Phase A current", nullptr, 0 },
    { "current_b", ExposeType::Numeric, ::zhc::Access::State, "A", "Phase B current", nullptr, 0 },
    { "current_c", ExposeType::Numeric, ::zhc::Access::State, "A", "Phase C current", nullptr, 0 },
    { "power_a",   ExposeType::Numeric, ::zhc::Access::State, "W", "Phase A active power", nullptr, 0 },
    { "power_b",   ExposeType::Numeric, ::zhc::Access::State, "W", "Phase B active power", nullptr, 0 },
    { "power_c",   ExposeType::Numeric, ::zhc::Access::State, "W", "Phase C active power", nullptr, 0 },
    { "energy_a",  ExposeType::Numeric, ::zhc::Access::State, "kWh", "Phase A consumed energy", nullptr, 0 },
    { "energy_b",  ExposeType::Numeric, ::zhc::Access::State, "kWh", "Phase B consumed energy", nullptr, 0 },
    { "energy_c",  ExposeType::Numeric, ::zhc::Access::State, "kWh", "Phase C consumed energy", nullptr, 0 },
    { "energy_produced_a", ExposeType::Numeric, ::zhc::Access::State, "kWh", "Phase A produced energy", nullptr, 0 },
    { "energy_produced_b", ExposeType::Numeric, ::zhc::Access::State, "kWh", "Phase B produced energy", nullptr, 0 },
    { "energy_produced_c", ExposeType::Numeric, ::zhc::Access::State, "kWh", "Phase C produced energy", nullptr, 0 },
    { "power_factor_a",    ExposeType::Numeric, ::zhc::Access::State, "%", "Phase A power factor", nullptr, 0 },
    { "power_factor_b",    ExposeType::Numeric, ::zhc::Access::State, "%", "Phase B power factor", nullptr, 0 },
    { "power_factor_c",    ExposeType::Numeric, ::zhc::Access::State, "%", "Phase C power factor", nullptr, 0 },
    // Diagnostics
    { "faults", ExposeType::String, ::zhc::Access::State, nullptr,
      "Active faults, comma separated; empty when none", nullptr, 0, ExposeCategory::Diagnostic },
    // Controls
    { "energy_reset", ExposeType::Binary, ::zhc::Access::Set, nullptr,
      "Write false to reset the energy counters (the wire value upstream sends for RESET)",
      nullptr, 0, ExposeCategory::Config },
    // Alarm flags and thresholds — read-only, see the header note on writes.
    { "over_current_alarm",      ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "over_current_threshold",  ExposeType::Numeric, ::zhc::Access::State, "A", nullptr, nullptr, 0, ExposeCategory::Config },
    { "unbalanced_load_alarm",     ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "unbalanced_load_threshold", ExposeType::Numeric, ::zhc::Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Config },
    { "over_voltage_alarm",      ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "over_voltage_threshold",  ExposeType::Numeric, ::zhc::Access::State, "V", nullptr, nullptr, 0, ExposeCategory::Config },
    { "under_voltage_alarm",     ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "under_voltage_threshold", ExposeType::Numeric, ::zhc::Access::State, "V", nullptr, nullptr, 0, ExposeCategory::Config },
    { "phase_loss_alarm",            ExposeType::Binary, ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "negative_active_power_alarm", ExposeType::Binary, ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "high_power_alarm",     ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "high_power_threshold", ExposeType::Numeric, ::zhc::Access::State, "W", nullptr, nullptr, 0, ExposeCategory::Config },
    { "custom_data_reporting_interval", ExposeType::Binary, ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "data_reporting_interval", ExposeType::Numeric, ::zhc::Access::State, nullptr,
      "Raw device value — z2m halves this before publishing", nullptr, 0, ExposeCategory::Config },
    { "device_locating",           ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "device_locating_threshold", ExposeType::Numeric, ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    // RS-485 gateway config
    { "rs485_baud_rate_enabled",   ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "rs485_baud_rate",           ExposeType::Numeric, ::zhc::Access::State, nullptr,
      "Raw exponent — the baud rate is 1200 * 2^value", nullptr, 0, ExposeCategory::Config },
    { "rs485_address_enabled",     ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "rs485_address",             ExposeType::Numeric, ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "rs485_data_format_enabled", ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "rs485_data_format",         ExposeType::Numeric, ::zhc::Access::State, nullptr,
      "Raw code: 81=N81, 82=E81, 83=O81, 84=N82", nullptr, 0, ExposeCategory::Config },
};

constexpr const char* kM_D4z[] = { "TS0601" };
constexpr const char* kN_D4z[] = {
    "_TZE204_loejka0i", "_TZE284_loejka0i", "_TZE200_loejka0i",
};

}  // namespace

extern const PreparedDefinition kDef_D4Z{
    .zigbee_models=kM_D4z, .zigbee_models_count=sizeof(kM_D4z)/sizeof(kM_D4z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_D4z, .manufacturer_names_count=sizeof(kN_D4z)/sizeof(kN_D4z[0]),
    .model="D4Z", .vendor="Nous",
    .meta=nullptr,
    .exposes=kExposes_D4Z, .exposes_count=sizeof(kExposes_D4Z)/sizeof(kExposes_D4Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FXD4z::fz_list, .from_zigbee_count=FXD4z::fz_count,
    .to_zigbee=FXD4z::tz_list, .to_zigbee_count=FXD4z::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::nous
