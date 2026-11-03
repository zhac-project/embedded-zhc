// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Vareo HYT001ZB floor-heating / boiler thermostat, TS0601
// _TZE284_48ye65rk (z2m v26.125.0 window). tuyaBase({dp, queryOnConfigure}).
// DPs: 16 local_temperature /10, 50 current_heating_setpoint /10, 102
// running_state (bool heat/idle), 104 holiday_days, 105 holiday_temperature,
// 109 local_temperature_calibration /10, 110 hysteresis /10, 116 sensor_type,
// 125 system_mode (bool heat/off), 128 preset, 130 fault bitmap. Flat climate
// exposes.
// z2m-source: tuya.ts #HYT001ZB.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kRunningState[] = { {1, "heat"}, {0, "idle"} };
constexpr ::zhc::tuya::TuyaEnumEntry kSystemMode[] = { {1, "heat"}, {0, "off"} };
constexpr ::zhc::tuya::TuyaEnumEntry kSensorType[] = { {0, "internal"}, {1, "external"}, {2, "both"} };
constexpr ::zhc::tuya::TuyaEnumEntry kPreset[] = { {0, "manual"}, {1, "program"}, {2, "holiday"}, {3, "temporary"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(16, "local_temperature", 10),
        ::zhc::tuya::dp::numeric(50, "current_heating_setpoint", 10),
        { 102, "running_state", ::zhc::TuyaDpType::Bool, 1, kRunningState, 2, ::zhc::tuya::kTuyaDpFlagBoolEnum },
        ::zhc::tuya::dp::numeric(104, "holiday_days", 1),
        ::zhc::tuya::dp::numeric(105, "holiday_temperature", 1),
        ::zhc::tuya::dp::numeric(109, "local_temperature_calibration", 10),
        ::zhc::tuya::dp::numeric(110, "hysteresis", 10),
        ::zhc::tuya::dp::enum_lookup(116, "sensor_type", kSensorType, 3),
        { 125, "system_mode", ::zhc::TuyaDpType::Bool, 1, kSystemMode, 2, ::zhc::tuya::kTuyaDpFlagBoolEnum },
        ::zhc::tuya::dp::enum_lookup(128, "preset", kPreset, 4),
        ::zhc::tuya::dp::numeric(130, "fault", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kSystemModeOpts[] = { "off", "heat" };
constexpr const char* kPresetOpts[] = { "manual", "program", "holiday", "temporary" };
constexpr const char* kRunningStateOpts[] = { "idle", "heat" };
constexpr const char* kSensorTypeOpts[] = { "internal", "external", "both" };
constexpr Expose kExp[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::Config, -9, 9, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kSystemModeOpts, 2},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kPresetOpts, 4},
    {"running_state", ExposeType::Enum, Access::State, nullptr, nullptr, kRunningStateOpts, 2},
    {"hysteresis", ExposeType::Numeric, Access::StateSet, "°C", "Switching hysteresis", nullptr, 0, ExposeCategory::Config, 0, 5, 0},
    {"sensor_type", ExposeType::Enum, Access::StateSet, nullptr, "Temperature sensor selection", kSensorTypeOpts, 3, ExposeCategory::Config},
    {"holiday_temperature", ExposeType::Numeric, Access::StateSet, "°C", "Setpoint in holiday mode", nullptr, 0, ExposeCategory::State, 5, 35, 1},
    {"holiday_days", ExposeType::Numeric, Access::StateSet, "d", "Holiday mode duration", nullptr, 0, ExposeCategory::State, 1, 30, 1},
    {"fault", ExposeType::Numeric, Access::State, nullptr, "Fault bitmap (4 = external sensor error)", nullptr, 0, ExposeCategory::Diagnostic},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_48ye65rk" };
}  // namespace

extern const PreparedDefinition kDef_HYT001ZB{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="HYT001ZB",.vendor="Vareo",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
