// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_cpbo62rn).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_cpbo62rn_state_dp1[] = {
    { 2, "CLOSE" },
    { 1, "STOP" },
    { 0, "OPEN" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_cpbo62rn_opening_mode_dp4[] = {
    { 0, "tilt" },
    { 1, "lift" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_cpbo62rn_work_state_dp7[] = {
    { 0, "standby" },
    { 1, "success" },
    { 2, "learning" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_cpbo62rn_motor_direction_dp101[] = {
    { 0, "left" },
    { 1, "right" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_cpbo62rn_set_upper_limit_dp102[] = {
    { 1, "start" },
    { 0, "stop" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_cpbo62rn[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_cpbo62rn_state_dp1, sizeof(kEnum__TZE200_cpbo62rn_state_dp1)/sizeof(kEnum__TZE200_cpbo62rn_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 4, "opening_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_cpbo62rn_opening_mode_dp4, sizeof(kEnum__TZE200_cpbo62rn_opening_mode_dp4)/sizeof(kEnum__TZE200_cpbo62rn_opening_mode_dp4[0]) },
    { 7, "work_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_cpbo62rn_work_state_dp7, sizeof(kEnum__TZE200_cpbo62rn_work_state_dp7)/sizeof(kEnum__TZE200_cpbo62rn_work_state_dp7[0]) },
    { 13, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "motor_direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_cpbo62rn_motor_direction_dp101, sizeof(kEnum__TZE200_cpbo62rn_motor_direction_dp101)/sizeof(kEnum__TZE200_cpbo62rn_motor_direction_dp101[0]) },
    { 102, "set_upper_limit", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_cpbo62rn_set_upper_limit_dp102, sizeof(kEnum__TZE200_cpbo62rn_set_upper_limit_dp102)/sizeof(kEnum__TZE200_cpbo62rn_set_upper_limit_dp102[0]) },
    { 104, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "factory_reset", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_cpbo62rn{ kEntries__TZE200_cpbo62rn, 10 };
constexpr FzConverter kFzDp__TZE200_cpbo62rn{
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
    .user_config       = &kMap__TZE200_cpbo62rn,
};
const FzConverter* const kFz__TZE200_cpbo62rn[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_cpbo62rn,
};
constexpr TzConverter kTzDp__TZE200_cpbo62rn{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_cpbo62rn,
};
const TzConverter* const kTz__TZE200_cpbo62rn[] = { &kTzDp__TZE200_cpbo62rn };
constexpr const char* kM__TZE200_cpbo62rn[] = { "TS0601" };
constexpr const char* kN__TZE200_cpbo62rn[] = { "_TZE200_cpbo62rn" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_cpbo62rn{
    .zigbee_models=kM__TZE200_cpbo62rn,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_cpbo62rn,.manufacturer_names_count=1,
    .model="TS0601__TZE200_cpbo62rn",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_cpbo62rn,
    .from_zigbee_count=sizeof(kFz__TZE200_cpbo62rn)/sizeof(kFz__TZE200_cpbo62rn[0]),
    .to_zigbee=kTz__TZE200_cpbo62rn,
    .to_zigbee_count=sizeof(kTz__TZE200_cpbo62rn)/sizeof(kTz__TZE200_cpbo62rn[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
