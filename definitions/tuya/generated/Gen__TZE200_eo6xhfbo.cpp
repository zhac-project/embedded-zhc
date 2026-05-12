// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_eo6xhfbo).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_eo6xhfbo_running_state_dp49[] = {
    { 0, "idle" },
    { 1, "heat" },
    { 2, "closing" },
    { 3, "opening" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_eo6xhfbo_screen_orientation_dp119[] = {
    { 0, "up" },
    { 1, "down" },
    { 2, "left" },
    { 3, "right" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_eo6xhfbo[] = {
    { 2, "preset", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 2, "system_mode", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 4, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 5, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 6, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 9, "max_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 10, "min_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 14, "window_detection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 15, "window_open", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 16, "open_window_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 28, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 29, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 30, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 31, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 32, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 33, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 34, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 35, "error_status", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 36, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 37, "boost_heating", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 38, "boost_timeset_countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 47, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 49, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_eo6xhfbo_running_state_dp49, sizeof(kEnum__TZE200_eo6xhfbo_running_state_dp49)/sizeof(kEnum__TZE200_eo6xhfbo_running_state_dp49[0]) },
    { 113, "switch_type", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 115, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 119, "screen_orientation", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_eo6xhfbo_screen_orientation_dp119, sizeof(kEnum__TZE200_eo6xhfbo_screen_orientation_dp119)/sizeof(kEnum__TZE200_eo6xhfbo_screen_orientation_dp119[0]) },
    { 1, "preheat", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "working_status", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "temperature_scale", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 11, "max_temperature_f", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 12, "min_temperature_f", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 17, "open_window_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "backlight", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 39, "switch_scale", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 40, "local_temperature_f", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 41, "current_heating_setpoint_f", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 46, "open_window_temperature_f", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 48, "valve_testing", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 112, "local_temperature_calibration_f", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 116, "frost_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 117, "frost_temperature_f", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 118, "auto_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 120, "multi_control_link", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 121, "holiday_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_eo6xhfbo{ kEntries__TZE200_eo6xhfbo, 45 };
constexpr FzConverter kFzDp__TZE200_eo6xhfbo{
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
    .user_config       = &kMap__TZE200_eo6xhfbo,
};
const FzConverter* const kFz__TZE200_eo6xhfbo[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_eo6xhfbo,
};
constexpr TzConverter kTzDp__TZE200_eo6xhfbo{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_eo6xhfbo,
};
const TzConverter* const kTz__TZE200_eo6xhfbo[] = { &kTzDp__TZE200_eo6xhfbo };
constexpr const char* kM__TZE200_eo6xhfbo[] = { "TS0601" };
constexpr const char* kN__TZE200_eo6xhfbo[] = { "_TZE200_eo6xhfbo" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_eo6xhfbo{
    .zigbee_models=kM__TZE200_eo6xhfbo,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_eo6xhfbo,.manufacturer_names_count=1,
    .model="TS0601__TZE200_eo6xhfbo",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_eo6xhfbo,
    .from_zigbee_count=sizeof(kFz__TZE200_eo6xhfbo)/sizeof(kFz__TZE200_eo6xhfbo[0]),
    .to_zigbee=kTz__TZE200_eo6xhfbo,
    .to_zigbee_count=sizeof(kTz__TZE200_eo6xhfbo)/sizeof(kTz__TZE200_eo6xhfbo[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
