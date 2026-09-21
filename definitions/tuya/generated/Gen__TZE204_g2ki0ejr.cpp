// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_g2ki0ejr).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_g2ki0ejr_running_state_dp49[] = {
    { 1, "heat" },
    { 0, "idle" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_g2ki0ejr_system_mode_dp49[] = {
    { 1, "heat" },
    { 0, "off" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_g2ki0ejr_preset_dp2[] = {
    { 3, "comfort" },
    { 0, "auto" },
    { 2, "manual" },
    { 1, "holiday" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_g2ki0ejr[] = {
    { 49, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_g2ki0ejr_running_state_dp49, sizeof(kEnum__TZE204_g2ki0ejr_running_state_dp49)/sizeof(kEnum__TZE204_g2ki0ejr_running_state_dp49[0]) },
    { 49, "system_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_g2ki0ejr_system_mode_dp49, sizeof(kEnum__TZE204_g2ki0ejr_system_mode_dp49)/sizeof(kEnum__TZE204_g2ki0ejr_system_mode_dp49[0]) },
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_g2ki0ejr_preset_dp2, sizeof(kEnum__TZE204_g2ki0ejr_preset_dp2)/sizeof(kEnum__TZE204_g2ki0ejr_preset_dp2[0]) },
    { 4, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 5, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 6, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 9, "max_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 10, "min_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 14, "window_detection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 16, "open_window_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 17, "open_window_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 19, "factory_reset", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 21, "holiday_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 24, "comfort_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 25, "eco_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 17, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 18, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 19, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 20, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 21, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 22, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 23, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 36, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 49, "valve", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_g2ki0ejr{ kEntries__TZE204_g2ki0ejr, 25 };
constexpr FzConverter kFzDp__TZE204_g2ki0ejr{
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
    .user_config       = &kMap__TZE204_g2ki0ejr,
};
const FzConverter* const kFz__TZE204_g2ki0ejr[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_g2ki0ejr,
};
constexpr TzConverter kTzDp__TZE204_g2ki0ejr{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_g2ki0ejr,
};
const TzConverter* const kTz__TZE204_g2ki0ejr[] = { &kTzDp__TZE204_g2ki0ejr };
constexpr const char* kM__TZE204_g2ki0ejr[] = { "TS0601" };
constexpr const char* kN__TZE204_g2ki0ejr[] = { "_TZE204_g2ki0ejr" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE204_g2ki0ejr_9[] = {"auto", "manual", "holiday", "comfort"};
constexpr const char* kAutoOpts__TZE204_g2ki0ejr_13[] = {"off", "heat"};
constexpr const char* kAutoOpts__TZE204_g2ki0ejr_14[] = {"idle", "heat"};
constexpr Expose kAutoExposes__TZE204_g2ki0ejr[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"max_temperature_limit", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 15, 45, 0},
    {"min_temperature_limit", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 15, 0},
    {"window_detection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"open_window_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 30, 0},
    {"comfort_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"eco_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"holiday_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_g2ki0ejr_9, 4},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -9, 9, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 45, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_g2ki0ejr_13, 2},
    {"running_state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_g2ki0ejr_14, 2},
    {"frost_protection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"boost_time", ExposeType::Numeric, Access::StateSet, "min", nullptr, nullptr, 0, ExposeCategory::Config, 0, 1440, 15},
    {"boost_timeset_countdown", ExposeType::Numeric, Access::State, "min", nullptr, nullptr, 0},
    {"schedule_monday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
    {"schedule_tuesday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
    {"schedule_wednesday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
    {"schedule_thursday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
    {"schedule_friday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
    {"schedule_saturday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
    {"schedule_sunday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
    {"valve", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"factory_reset", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"error_status", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_g2ki0ejr{
    .zigbee_models=kM__TZE204_g2ki0ejr,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_g2ki0ejr,.manufacturer_names_count=1,
    .model="TS0601__TZE204_g2ki0ejr",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_g2ki0ejr,.exposes_count=sizeof(kAutoExposes__TZE204_g2ki0ejr)/sizeof(kAutoExposes__TZE204_g2ki0ejr[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_g2ki0ejr,
    .from_zigbee_count=sizeof(kFz__TZE204_g2ki0ejr)/sizeof(kFz__TZE204_g2ki0ejr[0]),
    .to_zigbee=kTz__TZE204_g2ki0ejr,
    .to_zigbee_count=sizeof(kTz__TZE204_g2ki0ejr)/sizeof(kTz__TZE204_g2ki0ejr[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
