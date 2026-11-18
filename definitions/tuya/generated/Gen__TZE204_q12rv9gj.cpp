// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_q12rv9gj).
// HeatHUB HHST001. Hand-edited for z2m v26.133.0 (R10 W2, #13569): 4 manus, timeStart
// 1970, DP46 unit enum, DP101 "Temporary", DP114-117/119 settings. Not ported: the
// DP104 deadzone x10 scale of _6l44ypoc/_qvmmpbye, the DP19/26 conditional /10 and
// the DP106-112 DayNumber schedules.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_q12rv9gj_system_mode_dp2[] = {
    { 0, "cool" },
    { 1, "heat" },
    { 2, "fan_only" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_q12rv9gj_fan_mode_dp28[] = {
    { 0, "auto" },
    { 1, "high" },
    { 2, "medium" },
    { 3, "low" },
    { 4, "off" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_q12rv9gj_manual_mode_dp101[] = {
    { 0, "Auto" },
    { 1, "Manual" },
    { 2, "Temporary" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_q12rv9gj_temperature_scale_dp46[] = {
    { 0, "celsius" },
    { 1, "fahrenheit" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_q12rv9gj_lock_mode_dp119[] = {
    { 0, "semi_locked" },
    { 1, "full_locked" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_q12rv9gj[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "system_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_q12rv9gj_system_mode_dp2, sizeof(kEnum__TZE204_q12rv9gj_system_mode_dp2)/sizeof(kEnum__TZE204_q12rv9gj_system_mode_dp2[0]) },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 19, "max_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 18, "max_temperature_f", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 20, "min_temperature_f", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 21, "local_temperature_f", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 26, "min_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 102, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 103, "local_temperature_calibration_f", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "deadzone_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "deadzone_temperature_f", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 28, "fan_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_q12rv9gj_fan_mode_dp28, sizeof(kEnum__TZE204_q12rv9gj_fan_mode_dp28)/sizeof(kEnum__TZE204_q12rv9gj_fan_mode_dp28[0]) },
    { 40, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 44, "current_heating_setpoint_f", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 46, "temperature_scale", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_q12rv9gj_temperature_scale_dp46, 2 },
    { 101, "manual_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_q12rv9gj_manual_mode_dp101, sizeof(kEnum__TZE204_q12rv9gj_manual_mode_dp101)/sizeof(kEnum__TZE204_q12rv9gj_manual_mode_dp101[0]) },
    { 113, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    // z2m v26.126.0 (#13569) settings
    { 114, "water_drop_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 115, "day_standby_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 116, "night_standby_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 117, "humidity_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 119, "lock_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_q12rv9gj_lock_mode_dp119, 2 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_q12rv9gj{ kEntries__TZE204_q12rv9gj, sizeof(kEntries__TZE204_q12rv9gj)/sizeof(kEntries__TZE204_q12rv9gj[0]) };
constexpr FzConverter kFzDp__TZE204_q12rv9gj{
    .family            = FrameFamily::TuyaDp,
    .cluster           = "manuSpecificTuya",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .tuya_fn = &::zhc::tuya::fz_tuya_datapoints },
    .user_config       = &kMap__TZE204_q12rv9gj,
};
const FzConverter* const kFz__TZE204_q12rv9gj[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_q12rv9gj,
};
constexpr TzConverter kTzDp__TZE204_q12rv9gj{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_q12rv9gj,
};
const TzConverter* const kTz__TZE204_q12rv9gj[] = { &kTzDp__TZE204_q12rv9gj };
constexpr const char* kM__TZE204_q12rv9gj[] = { "TS0601" };
constexpr const char* kN__TZE204_q12rv9gj[] = {
    "_TZE204_q12rv9gj", "_TZE284_zgisddrg", "_TZE284_6l44ypoc", "_TZE284_ksqviprv", "_TZE284_qvmmpbye" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE204_q12rv9gj_2[] = {"cool", "heat", "fan_only"};
constexpr const char* kAutoOpts__TZE204_q12rv9gj_4[] = {"auto", "high", "medium", "low", "off"};
constexpr const char* kOptsManualMode[]  = {"Auto", "Manual", "Temporary"};
constexpr const char* kOptsTempScale[]   = {"celsius", "fahrenheit"};
constexpr const char* kOptsLockMode[]    = {"semi_locked", "full_locked"};
constexpr Expose kAutoExposes__TZE204_q12rv9gj[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_q12rv9gj_2, 3},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 45, 0},
    {"fan_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_q12rv9gj_4, 5},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -9, 9, 0},
    {"deadzone_temperature", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 5, 1},
    {"min_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 15, 0},
    {"max_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 35, 45, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"manual_mode", ExposeType::Enum, Access::StateSet, nullptr, "Schedule, manual or temporary mode", kOptsManualMode, 3},
    {"temperature_scale", ExposeType::Enum, Access::StateSet, nullptr, "Display and schedule unit", kOptsTempScale, 2},
    {"water_drop_switch", ExposeType::Binary, Access::StateSet, nullptr, "Water-drop switch", nullptr, 0},
    {"day_standby_brightness", ExposeType::Numeric, Access::StateSet, nullptr, "Daytime standby brightness", nullptr, 0, ExposeCategory::State, 0, 8, 1},
    {"night_standby_brightness", ExposeType::Numeric, Access::StateSet, nullptr, "Nighttime standby brightness", nullptr, 0, ExposeCategory::State, 0, 8, 1},
    {"humidity_calibration", ExposeType::Numeric, Access::StateSet, nullptr, "Humidity calibration, each step is 3%", nullptr, 0, ExposeCategory::State, -9, 9, 1},
    {"lock_mode", ExposeType::Enum, Access::StateSet, nullptr, "Child lock mode", kOptsLockMode, 2},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_q12rv9gj{
    .zigbee_models=kM__TZE204_q12rv9gj,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_q12rv9gj,.manufacturer_names_count=sizeof(kN__TZE204_q12rv9gj)/sizeof(kN__TZE204_q12rv9gj[0]),
    .model="TS0601__TZE204_q12rv9gj",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_q12rv9gj,.exposes_count=sizeof(kAutoExposes__TZE204_q12rv9gj)/sizeof(kAutoExposes__TZE204_q12rv9gj[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_q12rv9gj,
    .from_zigbee_count=sizeof(kFz__TZE204_q12rv9gj)/sizeof(kFz__TZE204_q12rv9gj[0]),
    .to_zigbee=kTz__TZE204_q12rv9gj,
    .to_zigbee_count=sizeof(kTz__TZE204_q12rv9gj)/sizeof(kTz__TZE204_q12rv9gj[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
    .tuya_time_start=1 };   // timeStart "1970" (z2m v26.126.0)
}  // namespace zhc::devices::tuya
