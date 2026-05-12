// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_6a4vxfnv).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_6a4vxfnv_preset_dp2[] = {
    { 0, "schedule" },
    { 1, "manual" },
    { 2, "away" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_6a4vxfnv_running_state_dp36[] = {
    { 0, "heat" },
    { 1, "idle" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_6a4vxfnv[] = {
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_6a4vxfnv_preset_dp2, sizeof(kEnum__TZE204_6a4vxfnv_preset_dp2)/sizeof(kEnum__TZE204_6a4vxfnv_preset_dp2[0]) },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 24, "device_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 27, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 36, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_6a4vxfnv_running_state_dp36, sizeof(kEnum__TZE204_6a4vxfnv_running_state_dp36)/sizeof(kEnum__TZE204_6a4vxfnv_running_state_dp36[0]) },
    { 40, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 103, "deadzone_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 110, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 109, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 108, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 107, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 106, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 105, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 101, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_6a4vxfnv{ kEntries__TZE204_6a4vxfnv, 15 };
constexpr FzConverter kFzDp__TZE204_6a4vxfnv{
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
    .user_config       = &kMap__TZE204_6a4vxfnv,
};
const FzConverter* const kFz__TZE204_6a4vxfnv[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_6a4vxfnv,
};
constexpr TzConverter kTzDp__TZE204_6a4vxfnv{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_6a4vxfnv,
};
const TzConverter* const kTz__TZE204_6a4vxfnv[] = { &kTzDp__TZE204_6a4vxfnv };
constexpr const char* kM__TZE204_6a4vxfnv[] = { "TS0601" };
constexpr const char* kN__TZE204_6a4vxfnv[] = { "_TZE204_6a4vxfnv" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_6a4vxfnv{
    .zigbee_models=kM__TZE204_6a4vxfnv,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_6a4vxfnv,.manufacturer_names_count=1,
    .model="TS0601__TZE204_6a4vxfnv",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_6a4vxfnv,
    .from_zigbee_count=sizeof(kFz__TZE204_6a4vxfnv)/sizeof(kFz__TZE204_6a4vxfnv[0]),
    .to_zigbee=kTz__TZE204_6a4vxfnv,
    .to_zigbee_count=sizeof(kTz__TZE204_6a4vxfnv)/sizeof(kTz__TZE204_6a4vxfnv[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
