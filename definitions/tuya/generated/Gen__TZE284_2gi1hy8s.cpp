// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_2gi1hy8s).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_2gi1hy8s_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_2gi1hy8s_motor_direction_dp11[] = {
    { 0, "normal" },
    { 1, "reversed" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_2gi1hy8s_set_limits_dp16[] = {
    { 0, "up" },
    { 1, "down" },
    { 2, "reset" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_2gi1hy8s[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_2gi1hy8s_state_dp1, sizeof(kEnum__TZE284_2gi1hy8s_state_dp1)/sizeof(kEnum__TZE284_2gi1hy8s_state_dp1[0]) },
    { 9, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 11, "motor_direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_2gi1hy8s_motor_direction_dp11, sizeof(kEnum__TZE284_2gi1hy8s_motor_direction_dp11)/sizeof(kEnum__TZE284_2gi1hy8s_motor_direction_dp11[0]) },
    { 13, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "set_limits", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_2gi1hy8s_set_limits_dp16, sizeof(kEnum__TZE284_2gi1hy8s_set_limits_dp16)/sizeof(kEnum__TZE284_2gi1hy8s_set_limits_dp16[0]) },
    { 101, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 103, "tilt_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_2gi1hy8s{ kEntries__TZE284_2gi1hy8s, 7 };
constexpr FzConverter kFzDp__TZE284_2gi1hy8s{
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
    .user_config       = &kMap__TZE284_2gi1hy8s,
};
const FzConverter* const kFz__TZE284_2gi1hy8s[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_2gi1hy8s,
};
constexpr TzConverter kTzDp__TZE284_2gi1hy8s{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_2gi1hy8s,
};
const TzConverter* const kTz__TZE284_2gi1hy8s[] = { &kTzDp__TZE284_2gi1hy8s };
constexpr const char* kM__TZE284_2gi1hy8s[] = { "TS0601" };
constexpr const char* kN__TZE284_2gi1hy8s[] = { "_TZE284_2gi1hy8s" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_2gi1hy8s{
    .zigbee_models=kM__TZE284_2gi1hy8s,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_2gi1hy8s,.manufacturer_names_count=1,
    .model="TS0601__TZE284_2gi1hy8s",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_2gi1hy8s,
    .from_zigbee_count=sizeof(kFz__TZE284_2gi1hy8s)/sizeof(kFz__TZE284_2gi1hy8s[0]),
    .to_zigbee=kTz__TZE284_2gi1hy8s,
    .to_zigbee_count=sizeof(kTz__TZE284_2gi1hy8s)/sizeof(kTz__TZE284_2gi1hy8s[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
