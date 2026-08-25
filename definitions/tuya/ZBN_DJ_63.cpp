// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: ZBN-DJ-63 DIN-rail smart breaker / prepayment energy meter
// (`_TZE204_lb0fsvba`) — z2m v26.99.0 parity (new upstream).
//
// PARTIAL PORT. 26 of the device's 32 datapoint rows are wired; three
// datapoints are deferred because they need packed-payload codecs that
// embedded-zhc does not have yet:
//
//   dp6  `phaseVariant2` — voltage, current and power packed into ONE
//        datapoint payload (2-byte V, 3-byte I, 3-byte P, big-endian). Needs a
//        multi-key unpacking DP type; `dp::numeric` emits a single key.
//   dp17 `threshold_2` and dp18 `threshold_3` — each packs FOUR or SIX keys
//        (a threshold value plus an on/off breaker flag per protection) into
//        one payload, and the WRITE path has to re-pack the other keys'
//        current values alongside the one being changed.
//
// So this def gives the meter's readings and switching (energy, state,
// countdown, temperature, prepayment balance, relay behaviour) but not live
// V/I/P nor the protection-threshold editors. Those three are the standing
// `phaseVariant2/5` + `threshold_2/3` framework gaps; when a codec lands,
// add the rows here.
//
// dp1/dp13/dp14/dp110 are energy counters in 0.01 kWh (divideBy100). dp103
// temperature is `raw` — whole degrees, NOT the usual x10.
//
// z2m-source: tuya.ts #ZBN-DJ-63.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kRelayPowerOnState[] = {
    {0, "off"}, {1, "on"}, {2, "memory"},
};

struct cfg_zbn_dj63 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "energy", 100),
        // dp6  — phaseVariant2 (packed V/I/P). DEFERRED, see header.
        ::zhc::tuya::dp::binary(11, "switch_prepayment"),
        ::zhc::tuya::dp::binary(12, "clear_energy"),
        ::zhc::tuya::dp::numeric(13, "balance_energy", 100),
        ::zhc::tuya::dp::numeric(14, "charge_energy", 100),
        ::zhc::tuya::dp::numeric(15, "leakage_current", 1),
        ::zhc::tuya::dp::binary(16, "state"),
        // dp17 — threshold_2 (leakage + high-temperature, packed). DEFERRED.
        // dp18 — threshold_3 (over-current / over- and under-voltage). DEFERRED.
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

constexpr const char* kRelayPowerOnOpts[] = { "off", "on", "memory" };

constexpr Expose kExp_ZbnDj63[] = {
    { "state",                    ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Breaker state",              nullptr, 0 },
    { "energy",                   ExposeType::Numeric, ::zhc::Access::State,    "kWh",   "Total energy",               nullptr, 0 },
    { "reverse_energy",           ExposeType::Numeric, ::zhc::Access::State,    "kWh",   "Reverse energy",             nullptr, 0 },
    { "temperature",              ExposeType::Numeric, ::zhc::Access::State,    "°C",    "Breaker temperature",        nullptr, 0 },
    { "leakage_current",          ExposeType::Numeric, ::zhc::Access::State,    "mA",    "Leakage current",            nullptr, 0 },
    { "status",                   ExposeType::Numeric, ::zhc::Access::State,    nullptr, "Breaker status word",        nullptr, 0,
      ExposeCategory::Diagnostic },
    { "alarm_over_current_count", ExposeType::Numeric, ::zhc::Access::State,    nullptr, "Over-current alarm count",   nullptr, 0,
      ExposeCategory::Diagnostic },
    { "alarm_low_current_count",  ExposeType::Numeric, ::zhc::Access::State,    nullptr, "Low-current alarm count",    nullptr, 0,
      ExposeCategory::Diagnostic },
    { "recover_count",            ExposeType::Numeric, ::zhc::Access::State,    nullptr, "Auto-recover attempt count", nullptr, 0,
      ExposeCategory::Diagnostic },
    { "switch_prepayment",        ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Prepayment mode",            nullptr, 0,
      ExposeCategory::Config },
    { "balance_energy",           ExposeType::Numeric, ::zhc::Access::State,    "kWh",   "Prepayment balance",         nullptr, 0 },
    { "charge_energy",            ExposeType::Numeric, ::zhc::Access::StateSet, "kWh",   "Prepayment top-up",          nullptr, 0,
      ExposeCategory::Config },
    { "clear_energy",             ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Clear the energy counter",   nullptr, 0,
      ExposeCategory::Config },
    { "recover_enable",           ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Auto-recover after a trip",  nullptr, 0,
      ExposeCategory::Config },
    { "countdown",                ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Switch-off countdown",       nullptr, 0,
      ExposeCategory::Config },
    { "cycle_time",               ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Cycle time",                 nullptr, 0,
      ExposeCategory::Config },
    { "leakage_delay",            ExposeType::Numeric, ::zhc::Access::StateSet, "ms",    "Leakage trip delay",         nullptr, 0,
      ExposeCategory::Config },
    { "power_on_delay",           ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Power-on delay",             nullptr, 0,
      ExposeCategory::Config },
    { "relay_power_on_state",     ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Relay state after power-on",
      kRelayPowerOnOpts, sizeof(kRelayPowerOnOpts)/sizeof(kRelayPowerOnOpts[0]), ExposeCategory::Config },
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
