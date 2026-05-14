// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_p6vz3wzt).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_p6vz3wzt_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_p6vz3wzt_calibration_dp3[] = {
    { 0, "START" },
    { 1, "END" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_p6vz3wzt_motor_steering_dp8[] = {
    { 0, "FORWARD" },
    { 1, "BACKWARD" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_p6vz3wzt[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_p6vz3wzt_state_dp1, sizeof(kEnum__TZE200_p6vz3wzt_state_dp1)/sizeof(kEnum__TZE200_p6vz3wzt_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "calibration", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_p6vz3wzt_calibration_dp3, sizeof(kEnum__TZE200_p6vz3wzt_calibration_dp3)/sizeof(kEnum__TZE200_p6vz3wzt_calibration_dp3[0]) },
    { 7, "backlight_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 8, "motor_steering", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_p6vz3wzt_motor_steering_dp8, sizeof(kEnum__TZE200_p6vz3wzt_motor_steering_dp8)/sizeof(kEnum__TZE200_p6vz3wzt_motor_steering_dp8[0]) },
    { 103, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_p6vz3wzt{ kEntries__TZE200_p6vz3wzt, 6 };
constexpr FzConverter kFzDp__TZE200_p6vz3wzt{
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
    .user_config       = &kMap__TZE200_p6vz3wzt,
};
const FzConverter* const kFz__TZE200_p6vz3wzt[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_p6vz3wzt,
};
constexpr TzConverter kTzDp__TZE200_p6vz3wzt{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_p6vz3wzt,
};
const TzConverter* const kTz__TZE200_p6vz3wzt[] = { &kTzDp__TZE200_p6vz3wzt };
constexpr const char* kM__TZE200_p6vz3wzt[] = { "TS0601" };
constexpr const char* kN__TZE200_p6vz3wzt[] = { "_TZE200_p6vz3wzt" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_p6vz3wzt[] = {
    {"Homeetec","37022483"},
    {"BSEED","_TZE284_uqfph8ah"},
};
extern const PreparedDefinition kDefGen__TZE200_p6vz3wzt{
    .zigbee_models=kM__TZE200_p6vz3wzt,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_p6vz3wzt,.manufacturer_names_count=1,
    .model="TS0601__TZE200_p6vz3wzt",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_p6vz3wzt, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_p6vz3wzt)/sizeof(kWhiteLabels_Gen_TZE200_p6vz3wzt[0]),
    .from_zigbee=kFz__TZE200_p6vz3wzt,
    .from_zigbee_count=sizeof(kFz__TZE200_p6vz3wzt)/sizeof(kFz__TZE200_p6vz3wzt[0]),
    .to_zigbee=kTz__TZE200_p6vz3wzt,
    .to_zigbee_count=sizeof(kTz__TZE200_p6vz3wzt)/sizeof(kTz__TZE200_p6vz3wzt[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
