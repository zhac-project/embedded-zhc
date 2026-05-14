// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_qyr2m29i).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_qyr2m29i_window_dp15[] = {
    { 0, "CLOSE" },
    { 1, "OPEN" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_qyr2m29i_motor_thrust_dp110[] = {
    { 0, "strong" },
    { 1, "middle" },
    { 2, "weak" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_qyr2m29i_display_brightness_dp111[] = {
    { 0, "high" },
    { 1, "medium" },
    { 2, "low" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_qyr2m29i_screen_orientation_dp113[] = {
    { 0, "up" },
    { 1, "down" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_qyr2m29i_hysteresis_dp127[] = {
    { 0, "comfort" },
    { 1, "eco" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_qyr2m29i[] = {
    { 2, "preset", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 2, "system_mode", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 4, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 5, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 6, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "max_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 10, "min_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 15, "window", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_qyr2m29i_window_dp15, sizeof(kEnum__TZE204_qyr2m29i_window_dp15)/sizeof(kEnum__TZE204_qyr2m29i_window_dp15[0]) },
    { 47, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 102, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 103, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 104, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 105, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 106, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 107, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 108, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 110, "motor_thrust", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_qyr2m29i_motor_thrust_dp110, sizeof(kEnum__TZE204_qyr2m29i_motor_thrust_dp110)/sizeof(kEnum__TZE204_qyr2m29i_motor_thrust_dp110[0]) },
    { 111, "display_brightness", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_qyr2m29i_display_brightness_dp111, sizeof(kEnum__TZE204_qyr2m29i_display_brightness_dp111)/sizeof(kEnum__TZE204_qyr2m29i_display_brightness_dp111[0]) },
    { 113, "screen_orientation", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_qyr2m29i_screen_orientation_dp113, sizeof(kEnum__TZE204_qyr2m29i_screen_orientation_dp113)/sizeof(kEnum__TZE204_qyr2m29i_screen_orientation_dp113[0]) },
    { 114, "position", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 118, "boost_timeset_countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 119, "comfort_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 120, "eco_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 121, "holiday_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 122, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 127, "hysteresis", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_qyr2m29i_hysteresis_dp127, sizeof(kEnum__TZE204_qyr2m29i_hysteresis_dp127)/sizeof(kEnum__TZE204_qyr2m29i_hysteresis_dp127[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_qyr2m29i{ kEntries__TZE204_qyr2m29i, 26 };
constexpr FzConverter kFzDp__TZE204_qyr2m29i{
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
    .user_config       = &kMap__TZE204_qyr2m29i,
};
const FzConverter* const kFz__TZE204_qyr2m29i[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_qyr2m29i,
};
constexpr TzConverter kTzDp__TZE204_qyr2m29i{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_qyr2m29i,
};
const TzConverter* const kTz__TZE204_qyr2m29i[] = { &kTzDp__TZE204_qyr2m29i };
constexpr const char* kM__TZE204_qyr2m29i[] = { "TS0601" };
constexpr const char* kN__TZE204_qyr2m29i[] = { "_TZE204_qyr2m29i" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_qyr2m29i[] = {
    {"Moes","TRV801Z"},
};
extern const PreparedDefinition kDefGen__TZE204_qyr2m29i{
    .zigbee_models=kM__TZE204_qyr2m29i,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_qyr2m29i,.manufacturer_names_count=1,
    .model="TS0601__TZE204_qyr2m29i",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_qyr2m29i, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_qyr2m29i)/sizeof(kWhiteLabels_Gen_TZE204_qyr2m29i[0]),
    .from_zigbee=kFz__TZE204_qyr2m29i,
    .from_zigbee_count=sizeof(kFz__TZE204_qyr2m29i)/sizeof(kFz__TZE204_qyr2m29i[0]),
    .to_zigbee=kTz__TZE204_qyr2m29i,
    .to_zigbee_count=sizeof(kTz__TZE204_qyr2m29i)/sizeof(kTz__TZE204_qyr2m29i[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
