// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Lincukoo G08A-Z20T sound + light siren with night light, TS0601
// _TZE204/_TZE200/_TZE28C1000000_euszleja (z2m v26.133.0 window).
// DPs: 1 alarm_state, 5 alarm_volume, 7 alarm_time, 16 mute, 19 alarm_mode,
// 21 alarm_ringtone, 23 night_light_mode, 101 night_light_brightness,
// 103 presence_hold_time, 104 play_ringtone (32 voice prompts).
// night_light_mode is a z2m binary("auto", "off") over an enum DP; Expose has
// no custom on/off labels, so it is exposed as a two-value enum.
// z2m-source: lincukoo.ts #G08A-Z20T.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::lincukoo {
namespace {

using ::zhc::tuya::TuyaEnumEntry;

constexpr TuyaEnumEntry kAlarmState[] = { {0, "alarm"}, {1, "normal"} };
constexpr TuyaEnumEntry kLowMidHigh[] = { {0, "low"}, {1, "middle"}, {2, "high"} };
constexpr TuyaEnumEntry kAlarmMode[]  = {
    {0, "sound_alarm"}, {1, "light_alarm"}, {2, "sound_light_alarm"}, {3, "disable_alarm"} };
constexpr TuyaEnumEntry kRingtone[]   = { {0, "ringtone_1"}, {1, "ringtone_2"}, {2, "ringtone_3"} };
constexpr TuyaEnumEntry kNightLight[] = { {0, "off"}, {1, "auto"} };
constexpr TuyaEnumEntry kPrompt[] = {
    {0, "warning_smoke_detected"}, {1, "warning_gas_leak_detected"},
    {2, "warning_carbon_monoxide_detected"}, {3, "warning_methane_detected"},
    {4, "fall_detected"}, {5, "abnormal_heart_rate"}, {6, "door_opened"},
    {7, "water_leak_detected"}, {8, "motion_detected"}, {9, "vibration_detected"},
    {10, "high_temperature"}, {11, "low_temperature"}, {12, "high_humidity"},
    {13, "low_humidity"}, {14, "soil_dry"}, {15, "high_liquid_level"},
    {16, "low_liquid_level"}, {17, "poor_water_quality"}, {18, "high_noise"},
    {19, "high_pressure"}, {20, "high_pm25_level"}, {21, "high_carbon_dioxide"},
    {22, "high_formaldehyde"}, {23, "high_voc_level"}, {24, "power_on"},
    {25, "power_off"}, {26, "light_on"}, {27, "light_off"},
    {28, "water_valve_open"}, {29, "water_valve_closed"}, {30, "curtain_open"},
    {31, "curtain_closed"},
};
constexpr std::uint8_t kPromptCount = sizeof(kPrompt) / sizeof(kPrompt[0]);

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "alarm_state", kAlarmState, 2),
        ::zhc::tuya::dp::enum_lookup(5, "alarm_volume", kLowMidHigh, 3),
        ::zhc::tuya::dp::numeric(7, "alarm_time"),
        ::zhc::tuya::dp::binary(16, "mute"),
        ::zhc::tuya::dp::enum_lookup(19, "alarm_mode", kAlarmMode, 4),
        ::zhc::tuya::dp::enum_lookup(21, "alarm_ringtone", kRingtone, 3),
        ::zhc::tuya::dp::enum_lookup(23, "night_light_mode", kNightLight, 2),
        ::zhc::tuya::dp::enum_lookup(101, "night_light_brightness", kLowMidHigh, 3),
        ::zhc::tuya::dp::numeric(103, "presence_hold_time"),
        ::zhc::tuya::dp::enum_lookup(104, "play_ringtone", kPrompt, kPromptCount),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kAlarmModeOpts[]  = { "sound_alarm", "light_alarm", "sound_light_alarm", "disable_alarm" };
constexpr const char* kAlarmStateOpts[] = { "alarm", "normal" };
constexpr const char* kLowMidHighOpts[] = { "low", "middle", "high" };
constexpr const char* kRingtoneOpts[]   = { "ringtone_1", "ringtone_2", "ringtone_3" };
constexpr const char* kNightLightOpts[] = { "auto", "off" };
constexpr const char* kPromptOpts[] = {
    "warning_smoke_detected", "warning_gas_leak_detected", "warning_carbon_monoxide_detected",
    "warning_methane_detected", "fall_detected", "abnormal_heart_rate", "door_opened",
    "water_leak_detected", "motion_detected", "vibration_detected", "high_temperature",
    "low_temperature", "high_humidity", "low_humidity", "soil_dry", "high_liquid_level",
    "low_liquid_level", "poor_water_quality", "high_noise", "high_pressure", "high_pm25_level",
    "high_carbon_dioxide", "high_formaldehyde", "high_voc_level", "power_on", "power_off",
    "light_on", "light_off", "water_valve_open", "water_valve_closed", "curtain_open",
    "curtain_closed",
};
static_assert(sizeof(kPromptOpts) / sizeof(kPromptOpts[0]) == kPromptCount);

constexpr Expose kExp[] = {
    {"alarm_mode", ExposeType::Enum, Access::StateSet, nullptr,
     "Alarm output mode: sound only / light only / sound + light / disabled", kAlarmModeOpts, 4},
    {"alarm_state", ExposeType::Enum, Access::StateSet, nullptr, "Alarm status", kAlarmStateOpts, 2},
    {"alarm_volume", ExposeType::Enum, Access::StateSet, nullptr, "Alarm volume", kLowMidHighOpts, 3},
    {"alarm_ringtone", ExposeType::Enum, Access::StateSet, nullptr, "Alarm ringtone", kRingtoneOpts, 3},
    {"alarm_time", ExposeType::Numeric, Access::StateSet, "times", "Alarm repeat count", nullptr, 0,
     ExposeCategory::State, 0, 100, 1},
    {"mute", ExposeType::Binary, Access::StateSet, nullptr, "Mute the siren", nullptr, 0},
    {"play_ringtone", ExposeType::Enum, Access::StateSet, nullptr, "Play a voice prompt", kPromptOpts, kPromptCount},
    {"night_light_mode", ExposeType::Enum, Access::StateSet, nullptr,
     "AUTO = on when any motion is detected, OFF = always off", kNightLightOpts, 2},
    {"night_light_brightness", ExposeType::Enum, Access::StateSet, nullptr, "Night light brightness", kLowMidHighOpts, 3},
    {"presence_hold_time", ExposeType::Numeric, Access::StateSet, "s",
     "Time the night light stays on after presence is gone", nullptr, 0, ExposeCategory::State, 5, 60, 5},
};

constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_euszleja", "_TZE200_euszleja", "_TZE28C1000000_euszleja" };

}  // namespace

extern const PreparedDefinition kDef_G08A_Z20T{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]), .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN, .manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="G08A-Z20T", .vendor="Lincukoo", .meta=nullptr,
    .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::lincukoo
