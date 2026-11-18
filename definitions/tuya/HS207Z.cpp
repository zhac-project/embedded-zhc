// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HYSYIOT HS207Z 5-in-1 PIR / illuminance / vibration / T+H sensor
// (z2m v26.133.0). Upstream fingerprints TS0601 and TS0202, each with all three
// `{_TZD200_,_TZE200_,_TZE204_}8sql5jcj`, so the manu x model cross product is
// exact; the bare zigbeeModel HS207Z is the second definition, kDef_HS207Z_model.
// ZCL: fz.ias_occupancy_alarm_1 (+ _report, attribute path not expressible,
// see tuya/TS0202.cpp) and fz.battery. DPs: 1 occupancy (trueFalse0),
// 3 battery_state, 4 battery, 9 pir_sensitivity, 11 illuminance, 12 pir_delay,
// 101 light_sensor_time, 102 vibration, 106 temperature_calibration /10,
// 107 indicator, 108 humidity_calibration, 109 temperature_unit,
// 110 temperature /10, 111 humidity, 112 vibration_sensitivity, 113 vibration_delay.
// z2m-source: tuya.ts #HS207Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kLowMiddleHigh[] = { {0,"low"}, {1,"middle"}, {2,"high"} };
constexpr ::zhc::tuya::TuyaEnumEntry kTemperatureUnit[] = { {0,"celsius"}, {1,"fahrenheit"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary_inv(1, "occupancy"),
        ::zhc::tuya::dp::enum_lookup(3, "battery_state", kLowMiddleHigh, 3),
        ::zhc::tuya::dp::numeric(4, "battery", 1),
        ::zhc::tuya::dp::enum_lookup(9, "pir_sensitivity", kLowMiddleHigh, 3),
        ::zhc::tuya::dp::numeric(11, "illuminance", 1),
        ::zhc::tuya::dp::numeric(12, "pir_delay", 1),
        ::zhc::tuya::dp::numeric(101, "light_sensor_time", 1),
        ::zhc::tuya::dp::binary(102, "vibration"),
        ::zhc::tuya::dp::numeric(106, "temperature_calibration", 10),
        ::zhc::tuya::dp::binary(107, "indicator"),
        ::zhc::tuya::dp::numeric(108, "humidity_calibration", 1),
        ::zhc::tuya::dp::enum_lookup(109, "temperature_unit", kTemperatureUnit, 2),
        ::zhc::tuya::dp::numeric(110, "temperature", 10),
        ::zhc::tuya::dp::numeric(111, "humidity", 1),
        ::zhc::tuya::dp::numeric(112, "vibration_sensitivity", 1),
        ::zhc::tuya::dp::numeric(113, "vibration_delay", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
const FzConverter* const kFz[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::tuya::factory::detail::BoundFz<cfg>::converter,
    &::zhc::generic::kFzIasMotionAlarm,   // occupancy + tamper + battery_low
    &::zhc::generic::kFzBattery,
};
constexpr const char* kOptsLMH[] = { "low", "middle", "high" };
constexpr const char* kOptsUnit[] = { "celsius", "fahrenheit" };
constexpr Expose kExp[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, "Indicates whether the device detected occupancy", nullptr, 0},
    {"vibration", ExposeType::Binary, Access::State, nullptr, "Indicates whether the device detected vibration", nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", "Measured illuminance in lux", nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", "Measured temperature", nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", "Measured relative humidity", nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", "Remaining battery in %", nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, "Indicates if the battery is low", nullptr, 0, ExposeCategory::Diagnostic},
    {"battery_state", ExposeType::Enum, Access::State, nullptr, "Battery level state: low, middle, or high", kOptsLMH, 3},
    {"pir_sensitivity", ExposeType::Enum, Access::StateSet, nullptr, "PIR detection sensitivity", kOptsLMH, 3},
    {"pir_delay", ExposeType::Numeric, Access::StateSet, "s", "PIR occupancy detection delay / keep time", nullptr, 0, ExposeCategory::State, 30, 120, 1},
    {"light_sensor_time", ExposeType::Numeric, Access::StateSet, nullptr, "Light sensor sampling interval", nullptr, 0, ExposeCategory::State, 1, 60, 1},
    // Upstream step is 0.5 °C; Expose bounds are integers, so the step is left unset.
    {"temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", "Temperature calibration offset", nullptr, 0, ExposeCategory::State, -4, 4, 0},
    {"indicator", ExposeType::Binary, Access::StateSet, nullptr, "LED indicator light switch", nullptr, 0},
    {"humidity_calibration", ExposeType::Numeric, Access::StateSet, "%", "Humidity calibration offset", nullptr, 0, ExposeCategory::State, -10, 10, 1},
    {"temperature_unit", ExposeType::Enum, Access::StateSet, nullptr, "Temperature display unit", kOptsUnit, 2},
    {"vibration_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr, "Vibration detection sensitivity", nullptr, 0, ExposeCategory::State, 1, 50, 1},
    {"vibration_delay", ExposeType::Numeric, Access::StateSet, "s", "Vibration delay recovery time", nullptr, 0, ExposeCategory::State, 1, 1440, 1},
};
constexpr const char* kM[] = { "TS0601", "TS0202" };
constexpr const char* kN[] = { "_TZD200_8sql5jcj", "_TZE200_8sql5jcj", "_TZE204_8sql5jcj" };
constexpr const char* kM_model[] = { "HS207Z" };
}  // namespace

extern const PreparedDefinition kDef_HS207Z{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="HS207Z",.vendor="HYSYIOT",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

// Upstream's zigbeeModel fingerprint: "HS207Z" with any manufacturer.
extern const PreparedDefinition kDef_HS207Z_model{
    .zigbee_models=kM_model,.zigbee_models_count=sizeof(kM_model)/sizeof(kM_model[0]),
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="HS207Z",.vendor="HYSYIOT",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
