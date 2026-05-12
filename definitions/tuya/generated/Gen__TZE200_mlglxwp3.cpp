// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_mlglxwp3).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_mlglxwp3_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_mlglxwp3_motor_direction_dp5[] = {
    { 0, "forward" },
    { 1, "back" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_mlglxwp3_work_state_dp7[] = {
    { 0, "opening" },
    { 1, "closing" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_mlglxwp3_situation_set_dp11[] = {
    { 0, "fully_close" },
    { 1, "fully_open" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_mlglxwp3[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_mlglxwp3_state_dp1, sizeof(kEnum__TZE200_mlglxwp3_state_dp1)/sizeof(kEnum__TZE200_mlglxwp3_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "motor_direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_mlglxwp3_motor_direction_dp5, sizeof(kEnum__TZE200_mlglxwp3_motor_direction_dp5)/sizeof(kEnum__TZE200_mlglxwp3_motor_direction_dp5[0]) },
    { 7, "work_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_mlglxwp3_work_state_dp7, sizeof(kEnum__TZE200_mlglxwp3_work_state_dp7)/sizeof(kEnum__TZE200_mlglxwp3_work_state_dp7[0]) },
    { 10, "total_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 11, "situation_set", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_mlglxwp3_situation_set_dp11, sizeof(kEnum__TZE200_mlglxwp3_situation_set_dp11)/sizeof(kEnum__TZE200_mlglxwp3_situation_set_dp11[0]) },
    { 12, "fault", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_mlglxwp3{ kEntries__TZE200_mlglxwp3, 9 };
constexpr FzConverter kFzDp__TZE200_mlglxwp3{
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
    .user_config       = &kMap__TZE200_mlglxwp3,
};
const FzConverter* const kFz__TZE200_mlglxwp3[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_mlglxwp3,
};
constexpr TzConverter kTzDp__TZE200_mlglxwp3{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_mlglxwp3,
};
const TzConverter* const kTz__TZE200_mlglxwp3[] = { &kTzDp__TZE200_mlglxwp3 };
constexpr const char* kM__TZE200_mlglxwp3[] = { "TS0601" };
constexpr const char* kN__TZE200_mlglxwp3[] = { "_TZE200_mlglxwp3" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_mlglxwp3{
    .zigbee_models=kM__TZE200_mlglxwp3,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_mlglxwp3,.manufacturer_names_count=1,
    .model="TS0601__TZE200_mlglxwp3",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_mlglxwp3,
    .from_zigbee_count=sizeof(kFz__TZE200_mlglxwp3)/sizeof(kFz__TZE200_mlglxwp3[0]),
    .to_zigbee=kTz__TZE200_mlglxwp3,
    .to_zigbee_count=sizeof(kTz__TZE200_mlglxwp3)/sizeof(kTz__TZE200_mlglxwp3[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
