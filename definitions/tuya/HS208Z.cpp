// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HYSYIOT HS208Z PIR + 24 GHz presence sensor (z2m v26.113.0, #13269).
// Upstream matches TS0202 + `_TZD200_sjjp9bti`, or the zigbeeModel HS208Z alone; the
// matcher's manufacturer list applies to every listed model, so the bare-model
// fingerprint is the second definition, kDef_HS208Z_model, in this file.
// z2m-source: tuya.ts #HS208Z.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kBatteryState3[] = { {0,"low"}, {1,"middle"}, {2,"high"} };
constexpr ::zhc::tuya::TuyaEnumEntry kPirSensitivity9[] = { {0,"low"}, {1,"middle"}, {2,"high"} };
constexpr ::zhc::tuya::TuyaEnumEntry kMotionState105[] = { {0,"none"}, {1,"move"}, {2,"Micro-move"}, {3,"static"} };
constexpr ::zhc::tuya::TuyaEnumEntry kTemperatureUnit109[] = { {0,"celsius"}, {1,"fahrenheit"} };
constexpr ::zhc::tuya::TuyaEnumEntry kMotionDetectionMode122[] = { {0,"pir_and_radar"}, {1,"pir_or_radar"}, {2,"only_radar"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary_inv(1, "occupancy"),
        { 3, "battery_state", ::zhc::TuyaDpType::Numeric, 1, kBatteryState3, 3, ::zhc::tuya::kTuyaDpFlagNumericLookup },
        ::zhc::tuya::dp::numeric(4, "battery", 1),
        { 9, "pir_sensitivity", ::zhc::TuyaDpType::Numeric, 1, kPirSensitivity9, 3, ::zhc::tuya::kTuyaDpFlagNumericLookup },
        ::zhc::tuya::dp::numeric(11, "illuminance", 1),
        ::zhc::tuya::dp::numeric(12, "pir_delay", 1),
        ::zhc::tuya::dp::numeric(101, "light_sensor_time", 1),
        ::zhc::tuya::dp::binary(102, "vibration"),
        ::zhc::tuya::dp::numeric(103, "detection_distance", 1),
        ::zhc::tuya::dp::numeric(104, "static_sensitivity", 1),
        { 105, "motion_state", ::zhc::TuyaDpType::Numeric, 1, kMotionState105, 4, ::zhc::tuya::kTuyaDpFlagNumericLookup },
        ::zhc::tuya::dp::numeric(106, "temperature_calibration", 10),
        ::zhc::tuya::dp::binary(107, "indicator"),
        ::zhc::tuya::dp::numeric(108, "humidity_calibration", 1),
        ::zhc::tuya::dp::enum_lookup(109, "temperature_unit", kTemperatureUnit109, 2),
        ::zhc::tuya::dp::numeric(110, "temperature", 10),
        ::zhc::tuya::dp::numeric(111, "humidity", 1),
        ::zhc::tuya::dp::numeric(112, "vibration_sensitivity", 1),
        ::zhc::tuya::dp::numeric(113, "vibration_delay", 1),
        ::zhc::tuya::dp::enum_lookup(122, "motion_detection_mode", kMotionDetectionMode122, 3),
        ::zhc::tuya::dp::numeric(123, "detection_sensitivity", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts1[] = { "none", "move", "Micro-move", "static" };
constexpr const char* kOpts7[] = { "celsius", "fahrenheit" };
constexpr const char* kOpts8[] = { "low", "middle", "high" };
constexpr const char* kOpts9[] = { "low", "middle", "high" };
constexpr const char* kOpts15[] = { "pir_and_radar", "pir_or_radar", "only_radar" };
constexpr Expose kExp[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"motion_state", ExposeType::Enum, Access::State, nullptr, "Radar Motion State Detail", kOpts1, 4},
    {"vibration", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"temperature_unit", ExposeType::Enum, Access::StateSet, nullptr, "Temperature unit", kOpts7, 2},
    {"battery_state", ExposeType::Enum, Access::State, nullptr, "Battery State", kOpts8, 3},
    {"pir_sensitivity", ExposeType::Enum, Access::StateSet, nullptr, "PIR Sensitivity", kOpts9, 3},
    {"pir_delay", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 30, 120, 0},
    {"light_sensor_time", ExposeType::Numeric, Access::StateSet, nullptr, "Light Sensor Sample Time", nullptr, 0, ExposeCategory::State, 1, 60, 0},
    {"detection_distance", ExposeType::Numeric, Access::StateSet, "cm", "Radar Detection Distance", nullptr, 0, ExposeCategory::State, 1, 500, 0},
    {"static_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr, "Static Detection Sensitivity", nullptr, 0, ExposeCategory::State, 1, 10, 0},
    {"detection_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr, "Motion Detection Sensitivity", nullptr, 0, ExposeCategory::State, 1, 10, 0},
    {"motion_detection_mode", ExposeType::Enum, Access::StateSet, nullptr, "Motion detection mode", kOpts15, 3},
    {"indicator", ExposeType::Binary, Access::StateSet, nullptr, "LED indicator mode", nullptr, 0},
    {"temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", "Temperature Calibration", nullptr, 0},
    {"humidity_calibration", ExposeType::Numeric, Access::StateSet, "%", "Humidity Calibration", nullptr, 0, ExposeCategory::State, -10, 10, 0},
    {"vibration_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr, "Vibration Sensitivity", nullptr, 0, ExposeCategory::State, 1, 50, 0},
    {"vibration_delay", ExposeType::Numeric, Access::StateSet, "s", "Vibration Clear Delay Time", nullptr, 0, ExposeCategory::State, 1, 1440, 0},
};
constexpr const char* kM[] = { "TS0202" };
constexpr const char* kN[] = { "_TZD200_sjjp9bti" };
}  // namespace

extern const PreparedDefinition kDef_HS208Z{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="HS208Z",.vendor="HYSYIOT",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya

namespace zhc::devices::tuya {
namespace {
constexpr const char* kM_HS208Z_model[] = { "HS208Z" };
}  // namespace

// Upstream's second fingerprint: zigbeeModel "HS208Z" with any manufacturer.
extern const PreparedDefinition kDef_HS208Z_model{
    .zigbee_models=kM_HS208Z_model,.zigbee_models_count=sizeof(kM_HS208Z_model)/sizeof(kM_HS208Z_model[0]),
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="HS208Z",.vendor="HYSYIOT",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
