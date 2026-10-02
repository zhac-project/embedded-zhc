// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tervix Zigbee thermostat (TS0601 / _TZE284_6kijc7nd, _TZE204_6kijc7nd),
// z2m v26.115.1 parity. Graduated from tuya/generated/Gen__TZE284_6kijc7nd.cpp
// and Gen__TZE204_6kijc7nd.cpp (identical copies).
//
// z2m v26.114.0 (#13329, "fix system_mode"): DP1 is a boolean datapoint,
// {off: false, heat: true}. The generated copies had dropped DP1 and every
// other plain-number lookup of upstream's table (mode, working_status,
// window_state, sensor_choose, run_mode), so the thermostat could not be
// switched on or off from the hub. Those ride the Numeric datapoint type
// upstream (plain numbers, no tuya.enum) — kTuyaDpFlagNumericLookup.
// DP48 week_schedule (168-byte raw) and DP61 week_program_periods are not
// ported; the schedule codec is not implemented upstream either.
// z2m-source: tuya.ts #_TZE284_6kijc7nd.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
using ::zhc::tuya::kTuyaDpFlagNumericLookup;
constexpr ::zhc::tuya::TuyaEnumEntry kSysMode[] = { {0,"off"}, {1,"heat"} };
constexpr ::zhc::tuya::TuyaEnumEntry kMode[]    = { {0,"manual"}, {1,"program"} };
constexpr ::zhc::tuya::TuyaEnumEntry kWork[]    = { {0,"Keeping Warm"}, {1,"Working"} };
constexpr ::zhc::tuya::TuyaEnumEntry kWindow[]  = { {1,"open"}, {0,"close"} };
constexpr ::zhc::tuya::TuyaEnumEntry kSensor[]  = { {0,"in"}, {1,"out"} };
constexpr ::zhc::tuya::TuyaEnumEntry kRunMode[] = { {1,"heat_mode"}, {2,"cool_mode"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        { 1, "system_mode", ::zhc::TuyaDpType::Bool, 1, kSysMode, 2, ::zhc::tuya::kTuyaDpFlagBoolEnum },
        { 2, "mode", ::zhc::TuyaDpType::Numeric, 1, kMode, 2, kTuyaDpFlagNumericLookup },
        { 3, "working_status", ::zhc::TuyaDpType::Numeric, 1, kWork, 2, kTuyaDpFlagNumericLookup },
        ::zhc::tuya::dp::binary(8, "window_check"),
        ::zhc::tuya::dp::binary(10, "frost_protection"),
        ::zhc::tuya::dp::numeric(16, "current_heating_setpoint", 10),
        ::zhc::tuya::dp::numeric(19, "upper_temp", 10),
        ::zhc::tuya::dp::numeric(24, "local_temperature", 10),
        { 25, "window_state", ::zhc::TuyaDpType::Numeric, 1, kWindow, 2, kTuyaDpFlagNumericLookup },
        ::zhc::tuya::dp::numeric(27, "temperature_correction", 1),
        ::zhc::tuya::dp::numeric(34, "humidity", 1),
        ::zhc::tuya::dp::binary(39, "factory_reset"),
        ::zhc::tuya::dp::binary(40, "child_lock"),
        { 43, "sensor_choose", ::zhc::TuyaDpType::Numeric, 1, kSensor, 2, kTuyaDpFlagNumericLookup },
        { 58, "run_mode", ::zhc::TuyaDpType::Numeric, 1, kRunMode, 2, kTuyaDpFlagNumericLookup },
        ::zhc::tuya::dp::numeric(101, "switch_sensitivity", 10),
        ::zhc::tuya::dp::numeric(102, "floor_temp_protection", 10),
        ::zhc::tuya::dp::numeric(103, "floor_low_protection", 10),
        ::zhc::tuya::dp::numeric(104, "window_open_detection_time", 1),
        ::zhc::tuya::dp::numeric(105, "window_open_detection_temp", 1),
        ::zhc::tuya::dp::numeric(106, "window_open_delay_time", 1),
        ::zhc::tuya::dp::binary(107, "humidity_control"),
        ::zhc::tuya::dp::numeric(108, "upper_humidity_limit", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kSysModeOpts[] = { "off", "heat" };
constexpr const char* kModeOpts[]    = { "manual", "program" };
constexpr const char* kRunModeOpts[] = { "heat_mode", "cool_mode" };
constexpr const char* kSensorOpts[]  = { "in", "out" };
constexpr const char* kWindowOpts[]  = { "open", "close" };
constexpr const char* kWorkOpts[]    = { "Keeping Warm", "Working" };
constexpr Expose kExp[] = {
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", "Temperature setpoint", nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"local_temperature",        ExposeType::Numeric, Access::State,    "°C", "Current temperature measured by the thermostat.", nullptr, 0},
    {"system_mode",              ExposeType::Enum,    Access::StateSet, nullptr, "Mode of this device", kSysModeOpts, 2},
    {"working_status",           ExposeType::Enum,    Access::State,    nullptr, "Heating state", kWorkOpts, 2},
    {"mode",                     ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kModeOpts, 2},
    {"run_mode",                 ExposeType::Enum,    Access::StateSet, nullptr, "Operation mode of the thermostat (heat or cool).", kRunModeOpts, 2},
    {"sensor_choose",            ExposeType::Enum,    Access::StateSet, nullptr, "Choose the temperature sensor", kSensorOpts, 2},
    {"window_check",             ExposeType::Binary,  Access::StateSet, nullptr, "Open window detection", nullptr, 0},
    {"window_state",             ExposeType::Enum,    Access::State,    nullptr, "Window state", kWindowOpts, 2},
    {"frost_protection",         ExposeType::Binary,  Access::StateSet, nullptr, "Frost protection", nullptr, 0},
    {"child_lock",               ExposeType::Binary,  Access::StateSet, nullptr, "Child lock", nullptr, 0},
    {"humidity",                 ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"upper_temp",               ExposeType::Numeric, Access::StateSet, "°C", "Upper temperature limit", nullptr, 0},
    {"temperature_correction",   ExposeType::Numeric, Access::StateSet, "°C", "Temperature correction", nullptr, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_6kijc7nd", "_TZE204_6kijc7nd" };
}  // namespace

extern const PreparedDefinition kDef_Tervix_6kijc7nd{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),.model="_TZE284_6kijc7nd",
    .vendor="Tervix",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
