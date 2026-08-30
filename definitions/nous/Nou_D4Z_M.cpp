// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Nous D4Z-M three-phase DIN-rail energy meter
// (`_TZE204_agjqiu4h` / `_TZE284_agjqiu4h` / `_TZE200_agjqiu4h`).
// New upstream in z2m v26.100.0; deferred at the time because every
// per-phase measurement rides a packed payload with no decoder in this tree.
//
// Those decoders now exist (see the packed-payload expanders in
// definitions/tuya/_shared.hpp), so the meter is fully wired on the read
// side:
//
//   dp6/7/8   phaseVariant2WithPhase — voltage, current and power for phases
//             A, B and C, packed 8 bytes per phase. Current and power are
//             24-bit big-endian; negative power is reported as
//             0x19999A + power rather than two's complement, so the sign is
//             recovered by an implausibility test, not a sign bit.
//   dp9       circuitBreakerFaults1 — a fault bitmap, joined into one
//             comma-separated `faults` string (z2m publishes a list; ZHC has
//             no list Value).
//   dp17/18   packed threshold records — [id, enabled, value_be16] tuples
//             covering the RS-485 config, the high-power alarm, and the
//             over/under voltage, over-current, unbalanced-load, phase-loss
//             and negative-power alarms.
//
// TWO UPSTREAM POST-PROCESSING STEPS ARE NOT REPRODUCED, both on dp17/18,
// and both are unit conversions on a value that is otherwise correct:
//   * `rs485_baud_rate` — z2m publishes `1200 * 2^raw`; here the raw
//     exponent is published.
//   * `data_reporting_interval` — z2m halves it; here the raw value is
//     published.
// The threshold expander is table-driven and deliberately has no per-key
// arithmetic hook; adding one for two RS-485 config fields on a single meter
// is not worth the surface. Both are recorded on the exposes below.
//
// WRITE PATH: the threshold datapoints are read-only here. Upstream's
// `encodeThresholds` is a read-modify-write over the WHOLE blob — it merges
// the key being set with the last published values of every sibling key and
// re-emits the entire record set. A ZHC tz converter gets the one key it was
// called with and 32 bytes of per-device scratch, which is not enough to hold
// both blobs (dp17 is 4 records, dp18 is 8 — 48 bytes together). Emitting
// only the changed record would probably work, since the device parses by
// record id and upstream itself skips records whose state it does not know,
// but "probably" is not a good enough basis for writing protection
// thresholds on a breaker. Left read-only until it can be tested on real
// hardware.
//
// z2m-source: nous.ts #D4Z-M.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::nous {
namespace {

constexpr ::zhc::tuya::TuyaPhaseKeys kPhaseA{ "voltage_a", "current_a", "power_a" };
constexpr ::zhc::tuya::TuyaPhaseKeys kPhaseB{ "voltage_b", "current_b", "power_b" };
constexpr ::zhc::tuya::TuyaPhaseKeys kPhaseC{ "voltage_c", "current_c", "power_c" };

// z2m `circuitBreakerFaultList`, indexed by BIT POSITION. Bit 14 is named
// "magnetism" upstream but published as "negative_power"; that substitution
// is applied here rather than at decode time.
constexpr const char* kFaultNames[] = {
    "short_circuit", "surge", "overload", "leakage_current",
    "temperature", "fire", "high_power", "self_test",
    "over_current", "unbalance", "over_voltage", "under_voltage",
    "miss_phase", "outage", "negative_power", "credit",
    "no_balance",
};
constexpr ::zhc::tuya::TuyaFaultTable kFaultTable{
    kFaultNames, sizeof(kFaultNames)/sizeof(kFaultNames[0]) };

// dp17 — z2m `alarmSet1BThresholdDefinitions`.
constexpr ::zhc::tuya::TuyaThresholdDef kAlarmSet1Defs[] = {
    { 3, "rs485_baud_rate_enabled",   "rs485_baud_rate"    },
    { 4, "rs485_address_enabled",     "rs485_address"      },
    { 5, "rs485_data_format_enabled", "rs485_data_format"  },
    { 7, "high_power_alarm",          "high_power_threshold" },
};
constexpr ::zhc::tuya::TuyaThresholdTable kAlarmSet1{
    kAlarmSet1Defs, sizeof(kAlarmSet1Defs)/sizeof(kAlarmSet1Defs[0]) };

// dp18 — z2m `alarmSet2ThresholdDefinitions`. Ids 5 and 7 are flag-only.
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

struct cfg_d4z_m {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "energy", 100),
        ::zhc::tuya::dp::phase_variant2_with_phase(6, &kPhaseA),
        ::zhc::tuya::dp::phase_variant2_with_phase(7, &kPhaseB),
        ::zhc::tuya::dp::phase_variant2_with_phase(8, &kPhaseC),
        ::zhc::tuya::dp::fault_bitmap(9, "faults", &kFaultTable),
        ::zhc::tuya::dp::thresholds(17, &kAlarmSet1),
        ::zhc::tuya::dp::thresholds(18, &kAlarmSet2),
        ::zhc::tuya::dp::numeric(23, "produced_energy", 100),
        ::zhc::tuya::dp::numeric(29, "power", 1),
        ::zhc::tuya::dp::numeric(32, "ac_frequency", 100),
        ::zhc::tuya::dp::numeric(50, "power_factor", 1),
        ::zhc::tuya::dp::numeric(53, "energy_a", 100),
        ::zhc::tuya::dp::numeric(54, "energy_b", 100),
        ::zhc::tuya::dp::numeric(55, "energy_c", 100),
        ::zhc::tuya::dp::numeric(57, "energy_produced_a", 100),
        ::zhc::tuya::dp::numeric(58, "energy_produced_b", 100),
        ::zhc::tuya::dp::numeric(59, "energy_produced_c", 100),
        ::zhc::tuya::dp::binary(101, "device_locating"),
        ::zhc::tuya::dp::numeric(102, "update_frequency", 1),
        ::zhc::tuya::dp::numeric(108, "power_factor_a", 1),
        ::zhc::tuya::dp::numeric(117, "power_factor_b", 1),
        ::zhc::tuya::dp::numeric(126, "power_factor_c", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXD4zM = ::zhc::tuya::factory::TuyaRw<cfg_d4z_m>;

constexpr Expose kExposes_D4Z_M[] = {
    // Totals
    { "energy",          ExposeType::Numeric, ::zhc::Access::State, "kWh", "Total forward active energy", nullptr, 0 },
    { "produced_energy", ExposeType::Numeric, ::zhc::Access::State, "kWh", "Total reverse active energy", nullptr, 0 },
    { "power",           ExposeType::Numeric, ::zhc::Access::State, "W",   "Total active power",          nullptr, 0 },
    { "power_factor",    ExposeType::Numeric, ::zhc::Access::State, "%",   "Total power factor",          nullptr, 0 },
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
    { "energy_a",  ExposeType::Numeric, ::zhc::Access::State, "kWh", "Phase A forward energy", nullptr, 0 },
    { "energy_b",  ExposeType::Numeric, ::zhc::Access::State, "kWh", "Phase B forward energy", nullptr, 0 },
    { "energy_c",  ExposeType::Numeric, ::zhc::Access::State, "kWh", "Phase C forward energy", nullptr, 0 },
    { "energy_produced_a", ExposeType::Numeric, ::zhc::Access::State, "kWh", "Phase A reverse energy", nullptr, 0 },
    { "energy_produced_b", ExposeType::Numeric, ::zhc::Access::State, "kWh", "Phase B reverse energy", nullptr, 0 },
    { "energy_produced_c", ExposeType::Numeric, ::zhc::Access::State, "kWh", "Phase C reverse energy", nullptr, 0 },
    { "power_factor_a",    ExposeType::Numeric, ::zhc::Access::State, "%", "Phase A power factor", nullptr, 0 },
    { "power_factor_b",    ExposeType::Numeric, ::zhc::Access::State, "%", "Phase B power factor", nullptr, 0 },
    { "power_factor_c",    ExposeType::Numeric, ::zhc::Access::State, "%", "Phase C power factor", nullptr, 0 },
    // Diagnostics
    { "faults", ExposeType::String, ::zhc::Access::State, nullptr,
      "Active faults, comma separated; empty when none", nullptr, 0, ExposeCategory::Diagnostic },
    // Controls
    { "device_locating",  ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Locate device", nullptr, 0, ExposeCategory::Config },
    { "update_frequency", ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Update frequency", nullptr, 0,
      ExposeCategory::Config, 5, 3600, 1 },
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

constexpr const char* kM_D4zM[] = { "TS0601" };
constexpr const char* kN_D4zM[] = {
    "_TZE204_agjqiu4h", "_TZE284_agjqiu4h", "_TZE200_agjqiu4h",
};

}  // namespace

extern const PreparedDefinition kDef_D4Z_M{
    .zigbee_models=kM_D4zM, .zigbee_models_count=sizeof(kM_D4zM)/sizeof(kM_D4zM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_D4zM, .manufacturer_names_count=sizeof(kN_D4zM)/sizeof(kN_D4zM[0]),
    .model="D4Z-M", .vendor="Nous",
    .meta=nullptr,
    .exposes=kExposes_D4Z_M, .exposes_count=sizeof(kExposes_D4Z_M)/sizeof(kExposes_D4Z_M[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FXD4zM::fz_list, .from_zigbee_count=FXD4zM::fz_count,
    .to_zigbee=FXD4zM::tz_list, .to_zigbee_count=FXD4zM::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::nous
