// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_mpbki2zm).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_mpbki2zm_fan_mode_dp28[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
    { 3, "auto" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_mpbki2zm[] = {
    { 4, "eco_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 19, "max_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 26, "min_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 27, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 28, "fan_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_mpbki2zm_fan_mode_dp28, sizeof(kEnum__TZE204_mpbki2zm_fan_mode_dp28)/sizeof(kEnum__TZE204_mpbki2zm_fan_mode_dp28[0]) },
    { 40, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 103, "deadzone_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "min_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 105, "max_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 106, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 107, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 108, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 109, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 110, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 111, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 112, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 101, "manual_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_mpbki2zm{ kEntries__TZE204_mpbki2zm, 19 };
constexpr FzConverter kFzDp__TZE204_mpbki2zm{
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
    .user_config       = &kMap__TZE204_mpbki2zm,
};
const FzConverter* const kFz__TZE204_mpbki2zm[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_mpbki2zm,
};
constexpr TzConverter kTzDp__TZE204_mpbki2zm{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_mpbki2zm,
};
const TzConverter* const kTz__TZE204_mpbki2zm[] = { &kTzDp__TZE204_mpbki2zm };
constexpr const char* kM__TZE204_mpbki2zm[] = { "TS0601" };
constexpr const char* kN__TZE204_mpbki2zm[] = { "_TZE204_mpbki2zm" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_mpbki2zm{
    .zigbee_models=kM__TZE204_mpbki2zm,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_mpbki2zm,.manufacturer_names_count=1,
    .model="TS0601__TZE204_mpbki2zm",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_mpbki2zm,
    .from_zigbee_count=sizeof(kFz__TZE204_mpbki2zm)/sizeof(kFz__TZE204_mpbki2zm[0]),
    .to_zigbee=kTz__TZE204_mpbki2zm,
    .to_zigbee_count=sizeof(kTz__TZE204_mpbki2zm)/sizeof(kTz__TZE204_mpbki2zm[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
