// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya ZG-227ZP temperature + humidity sensor with external NTC probe
// — new upstream in z2m v26.101.0.
//
// Matched by zigbeeModel (ZG-227ZP / ZG-227ZH) rather than a manufacturer
// fingerprint, unlike most TS0601 datapoint devices.
//
// Two temperature surfaces: the on-board sensor (dp1) and the plug-in NTC
// probe (dp105), each with its own calibration. The NTC also carries a
// high/low threshold pair and an alarm enum.
//
// z2m-source: tuya.ts #ZG-227ZP.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kUnit227[]  = { {0, "celsius"}, {1, "fahrenheit"} };
constexpr ::zhc::tuya::TuyaEnumEntry kNtcAlarm[] = {
    {0, "loweralarm"}, {1, "upperalarm"}, {2, "cancel"},
};

struct cfg_zg227 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "temperature", 10),
        ::zhc::tuya::dp::numeric(2, "humidity", 1),
        ::zhc::tuya::dp::numeric(4, "battery", 1),
        ::zhc::tuya::dp::numeric(6, "temperature_sampling", 1),
        ::zhc::tuya::dp::enum_lookup(9, "temperature_unit", kUnit227, 2),
        ::zhc::tuya::dp::numeric(10, "ntc_high_temp_alarm_threshold", 10),
        ::zhc::tuya::dp::numeric(11, "ntc_low_temp_alarm_threshold", 10),
        ::zhc::tuya::dp::enum_lookup(14, "ntc_alarm", kNtcAlarm, 3),
        ::zhc::tuya::dp::numeric(23, "temperature_calibration", 10),
        ::zhc::tuya::dp::numeric(24, "humidity_calibration", 1),
        ::zhc::tuya::dp::numeric(104, "ntc_temperature_calibration", 10),
        ::zhc::tuya::dp::numeric(105, "ntc_temperature", 10),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXZg227 = ::zhc::tuya::factory::TuyaRw<cfg_zg227>;

constexpr const char* kUnitOpts227[]  = { "celsius", "fahrenheit" };
constexpr const char* kNtcAlarmOpts[] = { "loweralarm", "upperalarm", "cancel" };

constexpr Expose kExposes_Zg227[] = {
    { "temperature",                   ExposeType::Numeric, ::zhc::Access::State,    "C", "On-board temperature", nullptr, 0 },
    { "humidity",                      ExposeType::Numeric, ::zhc::Access::State,    "%", "Humidity",             nullptr, 0 },
    { "ntc_temperature",               ExposeType::Numeric, ::zhc::Access::State,    "C", "External NTC probe temperature", nullptr, 0 },
    { "battery",                       ExposeType::Numeric, ::zhc::Access::State,    "%", "Battery", nullptr, 0, ExposeCategory::Diagnostic },
    { "ntc_alarm",                     ExposeType::Enum,    ::zhc::Access::State,    nullptr, "NTC probe alarm", kNtcAlarmOpts, 3 },
    { "temperature_unit",              ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Temperature unit", kUnitOpts227, 2, ExposeCategory::Config },
    { "temperature_sampling",          ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Sampling interval",   nullptr, 0, ExposeCategory::Config },
    { "temperature_calibration",       ExposeType::Numeric, ::zhc::Access::StateSet, "C", "On-board temperature calibration", nullptr, 0, ExposeCategory::Config },
    { "humidity_calibration",          ExposeType::Numeric, ::zhc::Access::StateSet, "%", "Humidity calibration", nullptr, 0, ExposeCategory::Config },
    { "ntc_temperature_calibration",   ExposeType::Numeric, ::zhc::Access::StateSet, "C", "NTC probe calibration", nullptr, 0, ExposeCategory::Config },
    { "ntc_high_temp_alarm_threshold", ExposeType::Numeric, ::zhc::Access::StateSet, "C", "NTC high-temperature alarm threshold", nullptr, 0, ExposeCategory::Config },
    { "ntc_low_temp_alarm_threshold",  ExposeType::Numeric, ::zhc::Access::StateSet, "C", "NTC low-temperature alarm threshold",  nullptr, 0, ExposeCategory::Config },
};

constexpr const char* kM_Zg227[] = { "ZG-227ZP", "ZG-227ZH" };

}  // namespace

extern const PreparedDefinition kDef_ZG_227ZP{
    .zigbee_models=kM_Zg227, .zigbee_models_count=sizeof(kM_Zg227)/sizeof(kM_Zg227[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZG-227ZP", .vendor="Tuya",
    .meta=nullptr,
    .exposes=kExposes_Zg227, .exposes_count=sizeof(kExposes_Zg227)/sizeof(kExposes_Zg227[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FXZg227::fz_list, .from_zigbee_count=FXZg227::fz_count,
    .to_zigbee=FXZg227::tz_list, .to_zigbee_count=FXZg227::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::tuya
