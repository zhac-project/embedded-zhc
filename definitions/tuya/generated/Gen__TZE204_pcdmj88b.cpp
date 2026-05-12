// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_pcdmj88b).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_pcdmj88b_preset_dp2[] = {
    { 0, "schedule" },
    { 1, "holiday" },
    { 2, "manual" },
    { 3, "comfort" },
    { 4, "eco" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_pcdmj88b_system_mode_dp49[] = {
    { 0, "off" },
    { 1, "heat" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_pcdmj88b[] = {
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_pcdmj88b_preset_dp2, sizeof(kEnum__TZE204_pcdmj88b_preset_dp2)/sizeof(kEnum__TZE204_pcdmj88b_preset_dp2[0]) },
    { 4, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 5, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 6, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 21, "holiday_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 24, "comfort_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 25, "eco_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 28, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 29, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 30, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 31, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 32, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 33, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 34, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 35, "fault_alarm", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 36, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 37, "boost_heating", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 39, "scale_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 47, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 49, "system_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_pcdmj88b_system_mode_dp49, sizeof(kEnum__TZE204_pcdmj88b_system_mode_dp49)/sizeof(kEnum__TZE204_pcdmj88b_system_mode_dp49[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_pcdmj88b{ kEntries__TZE204_pcdmj88b, 21 };
constexpr FzConverter kFzDp__TZE204_pcdmj88b{
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
    .user_config       = &kMap__TZE204_pcdmj88b,
};
const FzConverter* const kFz__TZE204_pcdmj88b[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_pcdmj88b,
};
constexpr TzConverter kTzDp__TZE204_pcdmj88b{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_pcdmj88b,
};
const TzConverter* const kTz__TZE204_pcdmj88b[] = { &kTzDp__TZE204_pcdmj88b };
constexpr const char* kM__TZE204_pcdmj88b[] = { "TS0601" };
constexpr const char* kN__TZE204_pcdmj88b[] = { "_TZE204_pcdmj88b" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_pcdmj88b{
    .zigbee_models=kM__TZE204_pcdmj88b,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_pcdmj88b,.manufacturer_names_count=1,
    .model="TS0601__TZE204_pcdmj88b",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_pcdmj88b,
    .from_zigbee_count=sizeof(kFz__TZE204_pcdmj88b)/sizeof(kFz__TZE204_pcdmj88b[0]),
    .to_zigbee=kTz__TZE204_pcdmj88b,
    .to_zigbee_count=sizeof(kTz__TZE204_pcdmj88b)/sizeof(kTz__TZE204_pcdmj88b[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
