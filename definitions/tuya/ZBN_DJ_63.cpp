// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: ZBN-DJ-63 DIN-rail smart breaker / prepayment energy meter
// (`_TZE204_lb0fsvba`) — z2m v26.101.0 parity.
//
// COMPLETED. The first port (v26.99.0 window) covered 26 of 32 datapoint
// rows and left the packed ones out because there were no codecs for them.
// Those codecs now exist — see the packed-payload expanders in
// definitions/tuya/_shared.hpp — so every datapoint upstream declares is
// wired:
//
//   dp6  `phaseVariant2`  -> voltage / current / power, packed into one
//        8-byte payload. NOTE this is upstream's NARROW variant: it reads
//        only the low two bytes of current and power, so current wraps above
//        65.536 A. That is upstream's behaviour for this device and is
//        reproduced rather than "fixed" — a 63 A breaker cannot reach the
//        wrap point anyway, and diverging would put this def out of step
//        with what z2m publishes for the same frame.
//   dp9  `fault` -> a single-fault enum. Upstream models it as a LOOKUP,
//        not a bitmap: the values are 0 and 1<<0 .. 1<<16, so exactly one
//        alarm decodes at a time and a multi-bit value matches nothing. That
//        is upstream's semantics and is what this table reproduces. (The
//        sibling D4Z-M meter uses the true bitmap converter for its own
//        fault datapoint — different device, different encoding.)
//   dp17 `threshold_2` -> overload / leakage / high-temperature breaker
//        enables and thresholds, packed as 4-byte records.
//   dp18 `threshold_3` -> over-current / over-voltage / under-voltage /
//        insufficient-balance breaker enables and thresholds.
//
// The `*_breaker` keys are Binary here where z2m publishes "ON"/"OFF"
// strings — ZHC's convention for a binary expose, consistent with every
// other definition in this tree.
//
// WRITE PATH for dp17/dp18: still read-only, deliberately. Upstream's
// encoder rebuilds the record for the key being set by pairing it with the
// LAST PUBLISHED value of its partner key (threshold with breaker, and vice
// versa) and re-sending both. A ZHC tz converter is handed one key and has
// no access to the shadow, so it cannot pair them; writing a threshold
// without its enable flag — or an enable flag with a zero threshold — would
// silently change breaker protection settings. Left read-only until the
// converter interface can carry the partner value.
//
// FIXED FROM THE FIRST PORT: `relay_power_on_state` had the labels
// off/on/memory. Upstream's lookup is {Off: 0, On: 1, Restore: 2} — the
// third state restores the pre-outage relay position rather than "last
// known", and the labels are capitalised. Any rule matching the old strings
// needs updating.
//
// z2m-source: tuya.ts #ZBN-DJ-63.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kRelayPowerOnState[] = {
    {0, "Off"}, {1, "On"}, {2, "Restore"},
};

// dp9 — upstream `valueConverterBasic.lookup`. Values are 0 and single bits;
// see the header note on why this is an enum and not a bitmap here.
constexpr ::zhc::tuya::TuyaEnumEntry kFaultLookup[] = {
    {0,          "clear"},
    {1u <<  0,   "short_circuit_alarm"},
    {1u <<  1,   "surge_alarm"},
    {1u <<  2,   "overload_alarm"},
    {1u <<  3,   "leakagecurr_alarm"},
    {1u <<  4,   "temp_dif_fault"},
    {1u <<  5,   "fire_alarm"},
    {1u <<  6,   "high_power_alarm"},
    {1u <<  7,   "self_test_alarm"},
    {1u <<  8,   "ov_cr"},
    {1u <<  9,   "unbalance_alarm"},
    {1u << 10,   "ov_vol"},
    {1u << 11,   "undervoltage_alarm"},
    {1u << 12,   "miss_phase_alarm"},
    {1u << 13,   "outage_alarm"},
    {1u << 14,   "magnetism_alarm"},
    {1u << 15,   "credit_alarm"},
    {1u << 16,   "no_balance_alarm"},
};

// dp17 — upstream `threshold_2.from` alarmLookup.
constexpr ::zhc::tuya::TuyaThresholdDef kThreshold2Defs[] = {
    { 3, "overload_breaker",         "overload_threshold"         },
    { 4, "leakage_breaker",          "leakage_threshold"          },
    { 5, "high_temperature_breaker", "high_temperature_threshold" },
};
constexpr ::zhc::tuya::TuyaThresholdTable kThreshold2{
    kThreshold2Defs, sizeof(kThreshold2Defs)/sizeof(kThreshold2Defs[0]) };

// dp18 — upstream `threshold_3.from` alarmLookup.
constexpr ::zhc::tuya::TuyaThresholdDef kThreshold3Defs[] = {
    { 1, "over_current_breaker",         "over_current_threshold"         },
    { 3, "over_voltage_breaker",         "over_voltage_threshold"         },
    { 4, "under_voltage_breaker",        "under_voltage_threshold"        },
    { 8, "insufficient_balance_breaker", "insufficient_balance_threshold" },
};
constexpr ::zhc::tuya::TuyaThresholdTable kThreshold3{
    kThreshold3Defs, sizeof(kThreshold3Defs)/sizeof(kThreshold3Defs[0]) };

struct cfg_zbn_dj63 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "energy", 100),
        ::zhc::tuya::dp::phase_variant2(6, &::zhc::tuya::kTuyaPhaseKeysPlain),
        ::zhc::tuya::dp::enum_lookup(9, "fault", kFaultLookup,
                                     sizeof(kFaultLookup)/sizeof(kFaultLookup[0])),
        ::zhc::tuya::dp::binary(11, "switch_prepayment"),
        ::zhc::tuya::dp::binary(12, "clear_energy"),
        ::zhc::tuya::dp::numeric(13, "balance_energy", 100),
        ::zhc::tuya::dp::numeric(14, "charge_energy", 100),
        ::zhc::tuya::dp::numeric(15, "leakage_current", 1),
        ::zhc::tuya::dp::binary(16, "state"),
        ::zhc::tuya::dp::thresholds(17, &kThreshold2),
        ::zhc::tuya::dp::thresholds(18, &kThreshold3),
        ::zhc::tuya::dp::numeric(102, "recover_count", 1),
        ::zhc::tuya::dp::numeric(103, "temperature", 1),
        ::zhc::tuya::dp::binary(104, "recover_enable"),
        ::zhc::tuya::dp::numeric(105, "countdown", 1),
        ::zhc::tuya::dp::numeric(106, "cycle_time", 1),
        ::zhc::tuya::dp::numeric(107, "leakage_delay", 1),
        ::zhc::tuya::dp::numeric(110, "reverse_energy", 100),
        ::zhc::tuya::dp::numeric(119, "power_on_delay", 1),
        ::zhc::tuya::dp::numeric(124, "alarm_over_current_count", 1),
        ::zhc::tuya::dp::numeric(125, "alarm_low_current_count", 1),
        ::zhc::tuya::dp::numeric(127, "status", 1),
        ::zhc::tuya::dp::enum_lookup(134, "relay_power_on_state", kRelayPowerOnState, 3),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXZbnDj63 = ::zhc::tuya::factory::TuyaRw<cfg_zbn_dj63>;

constexpr const char* kRelayPowerOnOpts[] = { "Off", "On", "Restore" };

constexpr const char* kFaultOpts[] = {
    "clear", "short_circuit_alarm", "surge_alarm", "overload_alarm",
    "leakagecurr_alarm", "temp_dif_fault", "fire_alarm", "high_power_alarm",
    "self_test_alarm", "ov_cr", "unbalance_alarm", "ov_vol",
    "undervoltage_alarm", "miss_phase_alarm", "outage_alarm",
    "magnetism_alarm", "credit_alarm", "no_balance_alarm",
};

constexpr Expose kExp_ZbnDj63[] = {
    // Live measurements — voltage/current/power all arrive on dp6.
    { "state",           ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Circuit breaker switch", nullptr, 0 },
    { "voltage",         ExposeType::Numeric, ::zhc::Access::State,    "V",   "Phase A voltage", nullptr, 0 },
    { "current",         ExposeType::Numeric, ::zhc::Access::State,    "A",   "Phase A current", nullptr, 0 },
    { "power",           ExposeType::Numeric, ::zhc::Access::State,    "W",   "Active power",    nullptr, 0 },
    { "energy",          ExposeType::Numeric, ::zhc::Access::State,    "kWh", "Total forward energy", nullptr, 0 },
    { "reverse_energy",  ExposeType::Numeric, ::zhc::Access::State,    "kWh", "Total reverse energy", nullptr, 0 },
    { "temperature",     ExposeType::Numeric, ::zhc::Access::State,    "C",   "Current temperature",  nullptr, 0 },
    { "leakage_current", ExposeType::Numeric, ::zhc::Access::State,    "mA",  "Residual current",     nullptr, 0 },
    { "fault",           ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Active fault",
      kFaultOpts, sizeof(kFaultOpts)/sizeof(kFaultOpts[0]), ExposeCategory::Diagnostic },
    { "status",          ExposeType::Numeric, ::zhc::Access::State,    nullptr, "Breaker status word", nullptr, 0,
      ExposeCategory::Diagnostic },
    { "alarm_over_current_count", ExposeType::Numeric, ::zhc::Access::State, nullptr, "Over-current alarm count", nullptr, 0,
      ExposeCategory::Diagnostic },
    { "alarm_low_current_count",  ExposeType::Numeric, ::zhc::Access::State, nullptr, "Low-current alarm count",  nullptr, 0,
      ExposeCategory::Diagnostic },
    { "recover_count",   ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Reclosing attempt limit", nullptr, 0,
      ExposeCategory::Config, 0, 999, 1 },
    // Prepayment
    { "switch_prepayment", ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Prepaid function", nullptr, 0,
      ExposeCategory::Config },
    { "balance_energy",    ExposeType::Numeric, ::zhc::Access::State,    "kWh", "Remaining available energy", nullptr, 0 },
    { "charge_energy",     ExposeType::Numeric, ::zhc::Access::Set,      "kWh", "Energy recharge", nullptr, 0,
      ExposeCategory::Config, 0, 999999, 1 },
    { "clear_energy",      ExposeType::Binary,  ::zhc::Access::Set,      nullptr, "Clear remaining available energy", nullptr, 0,
      ExposeCategory::Config },
    // Timers
    { "countdown",      ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Countdown timer", nullptr, 0,
      ExposeCategory::Config, 0, 43200, 1 },
    { "cycle_time",     ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Cycle time", nullptr, 0, ExposeCategory::Config },
    { "leakage_delay",  ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Leakage delay", nullptr, 0,
      ExposeCategory::Config, 1, 9999, 1 },
    { "power_on_delay", ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Power-on delay energization time", nullptr, 0,
      ExposeCategory::Config, 1, 9999, 1 },
    { "recover_enable", ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Reclosing enable", nullptr, 0,
      ExposeCategory::Config },
    { "relay_power_on_state", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Relay state after power-on",
      kRelayPowerOnOpts, sizeof(kRelayPowerOnOpts)/sizeof(kRelayPowerOnOpts[0]), ExposeCategory::Config },
    // Protection — read-only, see the header note on writes.
    { "over_current_breaker",   ExposeType::Binary,  ::zhc::Access::State, nullptr, "Overcurrent alarm enabled", nullptr, 0,
      ExposeCategory::Config },
    { "over_current_threshold", ExposeType::Numeric, ::zhc::Access::State, "A", "Overcurrent threshold", nullptr, 0,
      ExposeCategory::Config, 10, 63, 1 },
    { "over_voltage_breaker",   ExposeType::Binary,  ::zhc::Access::State, nullptr, "Overvoltage alarm enabled", nullptr, 0,
      ExposeCategory::Config },
    { "over_voltage_threshold", ExposeType::Numeric, ::zhc::Access::State, "V", "Overvoltage threshold", nullptr, 0,
      ExposeCategory::Config, 240, 260, 1 },
    { "under_voltage_breaker",  ExposeType::Binary,  ::zhc::Access::State, nullptr, "Undervoltage alarm enabled", nullptr, 0,
      ExposeCategory::Config },
    { "under_voltage_threshold",ExposeType::Numeric, ::zhc::Access::State, "V", "Undervoltage threshold", nullptr, 0,
      ExposeCategory::Config, 195, 220, 1 },
    { "leakage_breaker",        ExposeType::Binary,  ::zhc::Access::State, nullptr, "Leakage current alarm enabled", nullptr, 0,
      ExposeCategory::Config },
    { "leakage_threshold",      ExposeType::Numeric, ::zhc::Access::State, "mA", "Leakage current threshold", nullptr, 0,
      ExposeCategory::Config, 10, 99, 1 },
    { "high_temperature_breaker",   ExposeType::Binary,  ::zhc::Access::State, nullptr, "Temperature alarm enabled", nullptr, 0,
      ExposeCategory::Config },
    { "high_temperature_threshold", ExposeType::Numeric, ::zhc::Access::State, "C", "Temperature threshold", nullptr, 0,
      ExposeCategory::Config, 10, 85, 1 },
    // dp17 record id 3 and dp18 record id 8 decode too, though upstream
    // declares no expose for either. Kept so the values are not dropped.
    { "overload_breaker",   ExposeType::Binary,  ::zhc::Access::State, nullptr, "Overload alarm enabled", nullptr, 0,
      ExposeCategory::Config },
    { "overload_threshold", ExposeType::Numeric, ::zhc::Access::State, "W", "Overload threshold", nullptr, 0,
      ExposeCategory::Config },
    { "insufficient_balance_breaker",   ExposeType::Binary,  ::zhc::Access::State, nullptr,
      "Insufficient-balance alarm enabled", nullptr, 0, ExposeCategory::Config },
    { "insufficient_balance_threshold", ExposeType::Numeric, ::zhc::Access::State, "kWh",
      "Insufficient-balance threshold", nullptr, 0, ExposeCategory::Config },
};

constexpr const char* kM_ZbnDj63[] = { "TS0601" };
constexpr const char* kN_ZbnDj63[] = { "_TZE204_lb0fsvba" };

}  // namespace

extern const PreparedDefinition kDef_ZBN_DJ_63{
    .zigbee_models=kM_ZbnDj63, .zigbee_models_count=sizeof(kM_ZbnDj63)/sizeof(kM_ZbnDj63[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_ZbnDj63, .manufacturer_names_count=sizeof(kN_ZbnDj63)/sizeof(kN_ZbnDj63[0]),
    .model="ZBN-DJ-63", .vendor="Tuya",
    .meta=nullptr,
    .exposes=kExp_ZbnDj63, .exposes_count=sizeof(kExp_ZbnDj63)/sizeof(kExp_ZbnDj63[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FXZbnDj63::fz_list, .from_zigbee_count=FXZbnDj63::fz_count,
    .to_zigbee=FXZbnDj63::tz_list, .to_zigbee_count=FXZbnDj63::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::tuya
