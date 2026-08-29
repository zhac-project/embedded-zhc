// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: FrankEver FK-BV05 smart irrigation valve with flow metering.
//
// Not strictly a new device this window — upstream has carried it for a while
// and embedded-zhc simply never covered it. It surfaced during the
// v26.99 -> v26.101 sweep as an "uncovered" changed row, because v26.101.0
// added the two alarm-readback datapoints (114/115) and corrected dp5's
// divisor. Ported now rather than left as a standing gap.
//
// dp5 `water_consumed_last` is `divideBy10` as of v26.101.0 — it was `raw`
// before, so a pre-existing port would have read ten times high. The value is
// litres of the last irrigation run; dp6 is the lifetime total (raw litres).
//
// The alarm surface comes in pairs: an enable switch, a threshold, and (new in
// v26.101.0) a read-only flag saying the alarm is currently active.
//
// z2m-source: frankever.ts #FK-BV05.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::frankever {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kPowerOffState[] = {
    {0, "off"}, {1, "on"}, {2, "maintain"},
};

struct cfg_fk_bv05 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state"),
        ::zhc::tuya::dp::numeric(2, "threshold", 1),
        ::zhc::tuya::dp::numeric(3, "position", 1),
        ::zhc::tuya::dp::binary(4, "fault"),
        ::zhc::tuya::dp::numeric(5, "water_consumed_last", 10),
        ::zhc::tuya::dp::numeric(6, "water_consumed_total", 1),
        ::zhc::tuya::dp::numeric(11, "countdown", 1),
        ::zhc::tuya::dp::numeric(22, "water_temperature", 1),
        ::zhc::tuya::dp::binary(102, "single_irrigation_switch"),
        ::zhc::tuya::dp::numeric(103, "single_irrigation_set", 1),
        ::zhc::tuya::dp::binary(104, "day_irrigation_switch"),
        ::zhc::tuya::dp::numeric(105, "day_irrigation_set", 1),
        ::zhc::tuya::dp::binary(106, "water_volume_alarm_switch"),
        ::zhc::tuya::dp::numeric(107, "water_volume_alarm_set", 1),
        ::zhc::tuya::dp::binary(108, "water_temp_alarm_switch"),
        ::zhc::tuya::dp::numeric(109, "water_temp_alarm_max", 1),
        ::zhc::tuya::dp::enum_lookup(110, "power_off_state", kPowerOffState, 3),
        ::zhc::tuya::dp::binary(112, "creep_switch"),
        ::zhc::tuya::dp::numeric(113, "water_temp_alarm_min", 1),
        ::zhc::tuya::dp::binary(114, "water_temp_alarm"),
        ::zhc::tuya::dp::binary(115, "water_volume_alarm"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXFkBv05 = ::zhc::tuya::factory::TuyaRw<cfg_fk_bv05>;

constexpr const char* kPowerOffOpts[] = { "off", "on", "maintain" };

constexpr Expose kExposes_FkBv05[] = {
    { "state",                     ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Valve open",  nullptr, 0 },
    { "position",                  ExposeType::Numeric, ::zhc::Access::StateSet, "%", "Valve position", nullptr, 0, ExposeCategory::State, 0, 100, 1 },
    { "water_consumed_last",       ExposeType::Numeric, ::zhc::Access::State,    "L", "Water used by the last irrigation", nullptr, 0 },
    { "water_consumed_total",      ExposeType::Numeric, ::zhc::Access::State,    "L", "Lifetime water total", nullptr, 0 },
    { "water_temperature",         ExposeType::Numeric, ::zhc::Access::State,    "C", "Water temperature", nullptr, 0 },
    { "fault",                     ExposeType::Binary,  ::zhc::Access::State,    nullptr, "Fault detected", nullptr, 0, ExposeCategory::Diagnostic },
    { "water_temp_alarm",          ExposeType::Binary,  ::zhc::Access::State,    nullptr, "Water-temperature alarm is active", nullptr, 0 },
    { "water_volume_alarm",        ExposeType::Binary,  ::zhc::Access::State,    nullptr, "Water-volume alarm is active",      nullptr, 0 },
    { "countdown",                 ExposeType::Numeric, ::zhc::Access::StateSet, "min", "Irrigation countdown", nullptr, 0, ExposeCategory::Config },
    { "threshold",                 ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Flow threshold", nullptr, 0, ExposeCategory::Config },
    { "single_irrigation_switch",  ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Limit a single irrigation", nullptr, 0, ExposeCategory::Config },
    { "single_irrigation_set",     ExposeType::Numeric, ::zhc::Access::StateSet, "L", "Single-irrigation limit", nullptr, 0, ExposeCategory::Config },
    { "day_irrigation_switch",     ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Limit daily irrigation", nullptr, 0, ExposeCategory::Config },
    { "day_irrigation_set",        ExposeType::Numeric, ::zhc::Access::StateSet, "L", "Daily limit", nullptr, 0, ExposeCategory::Config },
    { "water_volume_alarm_switch", ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Enable the water-volume alarm", nullptr, 0, ExposeCategory::Config },
    { "water_volume_alarm_set",    ExposeType::Numeric, ::zhc::Access::StateSet, "L", "Water-volume alarm threshold", nullptr, 0, ExposeCategory::Config },
    { "water_temp_alarm_switch",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Enable the water-temperature alarm", nullptr, 0, ExposeCategory::Config },
    { "water_temp_alarm_max",      ExposeType::Numeric, ::zhc::Access::StateSet, "C", "High water-temperature alarm", nullptr, 0, ExposeCategory::Config },
    { "water_temp_alarm_min",      ExposeType::Numeric, ::zhc::Access::StateSet, "C", "Low water-temperature alarm",  nullptr, 0, ExposeCategory::Config },
    { "power_off_state",           ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Valve state after power loss", kPowerOffOpts, 3, ExposeCategory::Config },
    { "creep_switch",              ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Low-flow (creep) detection", nullptr, 0, ExposeCategory::Config },
};

constexpr const char* kM_FkBv05[] = { "TS0601" };
constexpr const char* kN_FkBv05[] = { "_TZE200_nbqnmkee" };

}  // namespace

extern const PreparedDefinition kDef_FK_BV05{
    .zigbee_models=kM_FkBv05, .zigbee_models_count=sizeof(kM_FkBv05)/sizeof(kM_FkBv05[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_FkBv05, .manufacturer_names_count=sizeof(kN_FkBv05)/sizeof(kN_FkBv05[0]),
    .model="FK-BV05", .vendor="FrankEver",
    .meta=nullptr,
    .exposes=kExposes_FkBv05, .exposes_count=sizeof(kExposes_FkBv05)/sizeof(kExposes_FkBv05[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FXFkBv05::fz_list, .from_zigbee_count=FXFkBv05::fz_count,
    .to_zigbee=FXFkBv05::tz_list, .to_zigbee_count=FXFkBv05::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::frankever
