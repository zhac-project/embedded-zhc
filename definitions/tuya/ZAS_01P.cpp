// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Novato ZAS-01P smart siren with night light (z2m v26.114.0, #13284).
// z2m-source: tuya.ts #ZAS-01P.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kAlarmState1[] = { {0,"alarm_sound"}, {1,"alarm_light"}, {2,"alarm_sound_light"}, {3,"normal"} };
constexpr ::zhc::tuya::TuyaEnumEntry kVolume5[] = { {0,"low"}, {1,"medium"}, {2,"high"} };
constexpr ::zhc::tuya::TuyaEnumEntry kMelody21[] = { {0,"doorbell"}, {1,"alarm_1"}, {2,"alarm_2"}, {3,"alarm_clock"}, {4,"notification"}, {5,"countdown"}, {6,"emergency_button"}, {7,"fall_detected"}, {8,"equipment_moved"}, {9,"carbon_dioxide"}, {10,"circuit_breaker"}, {11,"door_open"}, {12,"window_open"}, {13,"air_quality"}, {14,"motion_detected"}, {15,"person_detected"}, {16,"camera"}, {17,"vibration"}, {18,"ambient_temperature"}, {19,"target_temperature_reached"}, {20,"heating"}, {21,"water_level_alarm"}, {22,"valve_closed"}, {23,"scheduled_task"}, {24,"door_lock_alarm"}, {25,"smoke_alarm"}, {26,"gas_alarm"}, {27,"low_battery"}, {28,"water_leak_alarm"}, {29,"device_offline"}, {30,"alarm_system_disarmed"}, {31,"alarm_system_armed"} };
constexpr ::zhc::tuya::TuyaEnumEntry kLightMode23[] = { {0,"breathing"}, {1,"red_flash"}, {2,"white"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "alarm_state", kAlarmState1, 4),
        ::zhc::tuya::dp::enum_lookup(5, "volume", kVolume5, 3),
        ::zhc::tuya::dp::numeric(7, "duration", 1),
        ::zhc::tuya::dp::enum_lookup(21, "melody", kMelody21, 32),
        ::zhc::tuya::dp::binary(22, "night_light"),
        ::zhc::tuya::dp::enum_lookup(23, "light_mode", kLightMode23, 3),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts0[] = { "normal", "alarm_sound", "alarm_light", "alarm_sound_light" };
constexpr const char* kOpts1[] = { "low", "medium", "high" };
constexpr const char* kOpts3[] = { "doorbell", "alarm_1", "alarm_2", "alarm_clock", "notification", "countdown", "emergency_button", "fall_detected", "equipment_moved", "carbon_dioxide", "circuit_breaker", "door_open", "window_open", "air_quality", "motion_detected", "person_detected", "camera", "vibration", "ambient_temperature", "target_temperature_reached", "heating", "water_level_alarm", "valve_closed", "scheduled_task", "door_lock_alarm", "smoke_alarm", "gas_alarm", "low_battery", "water_leak_alarm", "device_offline", "alarm_system_disarmed", "alarm_system_armed" };
constexpr const char* kOpts5[] = { "breathing", "red_flash", "white" };
constexpr Expose kExp[] = {
    {"alarm_state", ExposeType::Enum, Access::StateSet, nullptr, "Trigger the alarm (sound, light or both) for the configured duration, or stop it with 'normal'", kOpts0, 4},
    {"volume", ExposeType::Enum, Access::StateSet, nullptr, "Alarm volume", kOpts1, 3},
    {"duration", ExposeType::Numeric, Access::StateSet, "s", "How long the alarm sounds for when triggered", nullptr, 0, ExposeCategory::State, 10, 1800, 1},
    {"melody", ExposeType::Enum, Access::StateSet, nullptr, "Alarm melody", kOpts3, 32},
    {"night_light", ExposeType::Binary, Access::StateSet, nullptr, "Night light", nullptr, 0},
    {"light_mode", ExposeType::Enum, Access::StateSet, nullptr, "Light mode", kOpts5, 3},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE20C_ycab9txf" };
}  // namespace

extern const PreparedDefinition kDef_ZAS_01P{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="ZAS-01P",.vendor="Novato",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
