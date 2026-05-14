// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_9mjy74mp).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_9mjy74mp_display_brightness_dp111[] = {
    { 0, "high" },
    { 1, "medium" },
    { 2, "low" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_9mjy74mp_screen_orientation_dp113[] = {
    { 0, "up" },
    { 1, "right" },
    { 2, "down" },
    { 3, "left" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_9mjy74mp_mode_dp114[] = {
    { 0, "comfort" },
    { 1, "eco" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_9mjy74mp[] = {
    { 1, "system_mode", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 1, "preset", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 2, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 3, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 8, "window_detection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 12, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 13, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "alarm_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 15, "min_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 16, "max_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 17, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 18, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 19, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 20, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 21, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 22, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 23, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 101, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 108, "position", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 111, "display_brightness", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_9mjy74mp_display_brightness_dp111, sizeof(kEnum__TZE204_9mjy74mp_display_brightness_dp111)/sizeof(kEnum__TZE204_9mjy74mp_display_brightness_dp111[0]) },
    { 113, "screen_orientation", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_9mjy74mp_screen_orientation_dp113, sizeof(kEnum__TZE204_9mjy74mp_screen_orientation_dp113)/sizeof(kEnum__TZE204_9mjy74mp_screen_orientation_dp113[0]) },
    { 114, "mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_9mjy74mp_mode_dp114, sizeof(kEnum__TZE204_9mjy74mp_mode_dp114)/sizeof(kEnum__TZE204_9mjy74mp_mode_dp114[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_9mjy74mp{ kEntries__TZE204_9mjy74mp, 22 };
constexpr FzConverter kFzDp__TZE204_9mjy74mp{
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
    .user_config       = &kMap__TZE204_9mjy74mp,
};
const FzConverter* const kFz__TZE204_9mjy74mp[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_9mjy74mp,
};
constexpr TzConverter kTzDp__TZE204_9mjy74mp{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_9mjy74mp,
};
const TzConverter* const kTz__TZE204_9mjy74mp[] = { &kTzDp__TZE204_9mjy74mp };
constexpr const char* kM__TZE204_9mjy74mp[] = { "TS0601" };
constexpr const char* kN__TZE204_9mjy74mp[] = { "_TZE204_9mjy74mp" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_9mjy74mp[] = {
    {"Moes","TRV801"},
};
extern const PreparedDefinition kDefGen__TZE204_9mjy74mp{
    .zigbee_models=kM__TZE204_9mjy74mp,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_9mjy74mp,.manufacturer_names_count=1,
    .model="TS0601__TZE204_9mjy74mp",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_9mjy74mp, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_9mjy74mp)/sizeof(kWhiteLabels_Gen_TZE204_9mjy74mp[0]),
    .from_zigbee=kFz__TZE204_9mjy74mp,
    .from_zigbee_count=sizeof(kFz__TZE204_9mjy74mp)/sizeof(kFz__TZE204_9mjy74mp[0]),
    .to_zigbee=kTz__TZE204_9mjy74mp,
    .to_zigbee_count=sizeof(kTz__TZE204_9mjy74mp)/sizeof(kTz__TZE204_9mjy74mp[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
