// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_lllliz3p).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_lllliz3p_preset_dp2[] = {
    { 0, "schedule" },
    { 1, "manual" },
    { 2, "away" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_lllliz3p_frost_protection_dp10[] = {
    { 0, "off" },
    { 1, "on" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_lllliz3p_working_day_dp31[] = {
    { 0, "mon_sun" },
    { 2, "separate" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_lllliz3p_system_mode_dp107[] = {
    { 0, "off" },
    { 1, "auto" },
    { 2, "heat" },
    { 3, "cool" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_lllliz3p_heating_stop_dp107[] = {
    { 0, "off" },
    { 1, "auto" },
    { 2, "heat" },
    { 3, "cool" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_lllliz3p[] = {
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_lllliz3p_preset_dp2, sizeof(kEnum__TZE200_lllliz3p_preset_dp2)/sizeof(kEnum__TZE200_lllliz3p_preset_dp2[0]) },
    { 8, "open_window", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 10, "frost_protection", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_lllliz3p_frost_protection_dp10, sizeof(kEnum__TZE200_lllliz3p_frost_protection_dp10)/sizeof(kEnum__TZE200_lllliz3p_frost_protection_dp10[0]) },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 27, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 31, "working_day", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_lllliz3p_working_day_dp31, sizeof(kEnum__TZE200_lllliz3p_working_day_dp31)/sizeof(kEnum__TZE200_lllliz3p_working_day_dp31[0]) },
    { 32, "holiday_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 35, "battery_low", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 40, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 45, "error_status", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "boost_timeset_countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "open_window_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 104, "comfort_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 105, "eco_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 106, "schedule", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 107, "system_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_lllliz3p_system_mode_dp107, sizeof(kEnum__TZE200_lllliz3p_system_mode_dp107)/sizeof(kEnum__TZE200_lllliz3p_system_mode_dp107[0]) },
    { 107, "heating_stop", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_lllliz3p_heating_stop_dp107, sizeof(kEnum__TZE200_lllliz3p_heating_stop_dp107)/sizeof(kEnum__TZE200_lllliz3p_heating_stop_dp107[0]) },
    { 115, "online", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 108, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 112, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 109, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 113, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 110, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 114, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 111, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_lllliz3p{ kEntries__TZE200_lllliz3p, 26 };
constexpr FzConverter kFzDp__TZE200_lllliz3p{
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
    .user_config       = &kMap__TZE200_lllliz3p,
};
const FzConverter* const kFz__TZE200_lllliz3p[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_lllliz3p,
};
constexpr TzConverter kTzDp__TZE200_lllliz3p{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_lllliz3p,
};
const TzConverter* const kTz__TZE200_lllliz3p[] = { &kTzDp__TZE200_lllliz3p };
constexpr const char* kM__TZE200_lllliz3p[] = { "TS0601" };
constexpr const char* kN__TZE200_lllliz3p[] = { "_TZE200_lllliz3p" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_lllliz3p{
    .zigbee_models=kM__TZE200_lllliz3p,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_lllliz3p,.manufacturer_names_count=1,
    .model="TS0601__TZE200_lllliz3p",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_lllliz3p,
    .from_zigbee_count=sizeof(kFz__TZE200_lllliz3p)/sizeof(kFz__TZE200_lllliz3p[0]),
    .to_zigbee=kTz__TZE200_lllliz3p,
    .to_zigbee_count=sizeof(kTz__TZE200_lllliz3p)/sizeof(kTz__TZE200_lllliz3p[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
