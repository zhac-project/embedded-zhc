// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tervix Pro Line Zigbee+RF wireless room thermostat 119710, TS0601
// _TZE284_xr5yfele (z2m v26.125.0 window). tuyaBase({dp, timeStart: "1970",
// forceTimeUpdates}): the 1970 time base is set on the def; the forced periodic
// time push is not reproduced (the hub answers the device's time requests).
// 31 DPs: raw / divideBy10 / onOff / lockUnlock / lookups; DP27 calibration is
// signed raw. Flat climate exposes.
// z2m-source: tuya.ts #119710.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSystemMode[] = { {0, "off"}, {1, "heat"} };
constexpr ::zhc::tuya::TuyaEnumEntry kPreset[] = { {0, "manual"}, {1, "program"} };
constexpr ::zhc::tuya::TuyaEnumEntry kOnOffWords[] = { {0, "OFF"}, {1, "ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kWindowState[] = { {0, "close"}, {1, "open"} };
constexpr ::zhc::tuya::TuyaEnumEntry kRunningState[] = { {0, "idle"}, {1, "heat"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        { 1, "system_mode", ::zhc::TuyaDpType::Bool, 1, kSystemMode, 2, ::zhc::tuya::kTuyaDpFlagBoolEnum },
        ::zhc::tuya::dp::enum_lookup(2, "preset", kPreset, 2),
        ::zhc::tuya::dp::enum_lookup(3, "backlight", kOnOffWords, 2),
        ::zhc::tuya::dp::numeric(8, "humidity", 1),
        ::zhc::tuya::dp::numeric(9, "battery", 1),
        ::zhc::tuya::dp::numeric(16, "local_temperature", 10),
        ::zhc::tuya::dp::enum_lookup(17, "window_state", kWindowState, 2),
        ::zhc::tuya::dp::numeric(27, "local_temperature_calibration", 1),
        ::zhc::tuya::dp::binary(29, "window_detection"),
        ::zhc::tuya::dp::binary(31, "frost_protection"),
        ::zhc::tuya::dp::binary(39, "child_lock"),
        ::zhc::tuya::dp::numeric(50, "current_heating_setpoint", 10),
        ::zhc::tuya::dp::numeric(101, "valve_init_opening", 1),
        ::zhc::tuya::dp::numeric(102, "comfort_temperature", 10),
        ::zhc::tuya::dp::numeric(103, "pid_p", 1),
        ::zhc::tuya::dp::numeric(104, "valve_opening", 1),
        ::zhc::tuya::dp::numeric(105, "eco_temperature", 10),
        ::zhc::tuya::dp::numeric(106, "window_open_detection_time", 1),
        ::zhc::tuya::dp::numeric(107, "window_open_detection_temperature", 10),
        ::zhc::tuya::dp::numeric(108, "window_open_delay_time", 1),
        ::zhc::tuya::dp::numeric(109, "boost_time", 1),
        ::zhc::tuya::dp::numeric(110, "pid_i", 1),
        ::zhc::tuya::dp::numeric(111, "trv_count", 1),
        ::zhc::tuya::dp::binary(112, "quick_pair"),
        ::zhc::tuya::dp::binary(113, "display_reversed"),
        ::zhc::tuya::dp::binary(115, "humidity_control"),
        ::zhc::tuya::dp::numeric(116, "humidity_setpoint", 1),
        ::zhc::tuya::dp::enum_lookup(118, "running_state", kRunningState, 2),
        ::zhc::tuya::dp::numeric(119, "heat_times_today", 1),
        ::zhc::tuya::dp::numeric(120, "heat_times_total", 1),
        ::zhc::tuya::dp::numeric(121, "heat_times_month", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kSystemModeOpts[] = { "off", "heat" };
constexpr const char* kRunningStateOpts[] = { "idle", "heat" };
constexpr const char* kPresetOpts[] = { "manual", "program" };
constexpr const char* kOnOffOpts[] = { "ON", "OFF" };
constexpr const char* kWindowStateOpts[] = { "close", "open" };
constexpr Expose kExp[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::Config, -9, 9, 1},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kSystemModeOpts, 2},
    {"running_state", ExposeType::Enum, Access::State, nullptr, nullptr, kRunningStateOpts, 2},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kPresetOpts, 2},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
    {"backlight", ExposeType::Enum, Access::StateSet, nullptr, "Display backlight", kOnOffOpts, 2},
    {"window_state", ExposeType::Enum, Access::State, nullptr, "Window state", kWindowStateOpts, 2},
    {"window_detection", ExposeType::Binary, Access::StateSet, nullptr, "Open window detection", nullptr, 0},
    {"frost_protection", ExposeType::Binary, Access::StateSet, nullptr, "Frost protection", nullptr, 0},
    {"comfort_temperature", ExposeType::Numeric, Access::StateSet, "°C", "Comfort temperature", nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"eco_temperature", ExposeType::Numeric, Access::StateSet, "°C", "Eco temperature", nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"window_open_detection_time", ExposeType::Numeric, Access::StateSet, "min", "Open window detection time", nullptr, 0, ExposeCategory::State, 0, 1440, 1},
    {"window_open_detection_temperature", ExposeType::Numeric, Access::StateSet, "°C", "Open window detection temperature drop", nullptr, 0, ExposeCategory::State, 0, 35, 0},
    {"window_open_delay_time", ExposeType::Numeric, Access::StateSet, "min", "Open window detection delay", nullptr, 0, ExposeCategory::State, 0, 1440, 1},
    {"boost_time", ExposeType::Numeric, Access::StateSet, "min", "Boost duration", nullptr, 0, ExposeCategory::State, 0, 1440, 1},
    {"valve_init_opening", ExposeType::Numeric, Access::StateSet, "%", "Initial valve opening", nullptr, 0, ExposeCategory::State, 0, 100, 1},
    {"valve_opening", ExposeType::Numeric, Access::State, "%", "Current valve opening", nullptr, 0},
    {"pid_p", ExposeType::Numeric, Access::StateSet, nullptr, "PID proportional coefficient", nullptr, 0, ExposeCategory::State, 0, 100, 1},
    {"pid_i", ExposeType::Numeric, Access::StateSet, nullptr, "PID integral coefficient", nullptr, 0, ExposeCategory::State, 0, 100, 1},
    {"trv_count", ExposeType::Numeric, Access::State, nullptr, "Number of paired TRVs", nullptr, 0},
    {"quick_pair", ExposeType::Binary, Access::StateSet, nullptr, "Quick pairing of RF TRVs", nullptr, 0},
    {"display_reversed", ExposeType::Binary, Access::StateSet, nullptr, "Reverse display orientation", nullptr, 0},
    {"humidity_control", ExposeType::Binary, Access::StateSet, nullptr, "Humidity control", nullptr, 0},
    {"humidity_setpoint", ExposeType::Numeric, Access::StateSet, "%", "Humidity setpoint", nullptr, 0, ExposeCategory::State, 0, 100, 1},
    {"heat_times_today", ExposeType::Numeric, Access::State, nullptr, "Heating cycles today", nullptr, 0},
    {"heat_times_month", ExposeType::Numeric, Access::State, nullptr, "Heating cycles this month", nullptr, 0},
    {"heat_times_total", ExposeType::Numeric, Access::State, nullptr, "Total heating cycles", nullptr, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_xr5yfele" };
}  // namespace

extern const PreparedDefinition kDef_T119710{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="119710",.vendor="Tervix",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr,
    .tuya_time_start=1 };

}  // namespace zhc::devices::tuya
