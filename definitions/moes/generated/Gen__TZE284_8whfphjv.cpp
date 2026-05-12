// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_8whfphjv).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_8whfphjv_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_8whfphjv_motor_direction_dp11[] = {
    { 0, "forward" },
    { 1, "back" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_8whfphjv[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_8whfphjv_state_dp1, sizeof(kEnum__TZE284_8whfphjv_state_dp1)/sizeof(kEnum__TZE284_8whfphjv_state_dp1[0]) },
    { 9, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 8, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 11, "motor_direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_8whfphjv_motor_direction_dp11, sizeof(kEnum__TZE284_8whfphjv_motor_direction_dp11)/sizeof(kEnum__TZE284_8whfphjv_motor_direction_dp11[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_8whfphjv{ kEntries__TZE284_8whfphjv, 4 };
constexpr FzConverter kFzDp__TZE284_8whfphjv{
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
    .user_config       = &kMap__TZE284_8whfphjv,
};
const FzConverter* const kFz__TZE284_8whfphjv[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_8whfphjv,
};
constexpr TzConverter kTzDp__TZE284_8whfphjv{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_8whfphjv,
};
const TzConverter* const kTz__TZE284_8whfphjv[] = { &kTzDp__TZE284_8whfphjv };
constexpr const char* kM__TZE284_8whfphjv[] = { "TS0601" };
constexpr const char* kN__TZE284_8whfphjv[] = { "_TZE284_8whfphjv" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_8whfphjv{
    .zigbee_models=kM__TZE284_8whfphjv,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_8whfphjv,.manufacturer_names_count=1,
    .model="TS0601__TZE284_8whfphjv",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_8whfphjv,
    .from_zigbee_count=sizeof(kFz__TZE284_8whfphjv)/sizeof(kFz__TZE284_8whfphjv[0]),
    .to_zigbee=kTz__TZE284_8whfphjv,
    .to_zigbee_count=sizeof(kTz__TZE284_8whfphjv)/sizeof(kTz__TZE284_8whfphjv[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
