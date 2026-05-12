// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_dq8bu0pt).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_dq8bu0pt_mode_l1_dp18[] = {
    { 0, "switch_1" },
    { 1, "scene_1" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_dq8bu0pt_mode_l2_dp19[] = {
    { 0, "switch_2" },
    { 1, "scene_2" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_dq8bu0pt_mode_l3_dp20[] = {
    { 0, "switch_3" },
    { 1, "scene_3" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_dq8bu0pt_mode_l4_dp21[] = {
    { 0, "switch_4" },
    { 1, "scene_4" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_dq8bu0pt_indicator_status_dp37[] = {
    { 0, "off" },
    { 1, "relay" },
    { 2, "invert" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_dq8bu0pt_power_on_behavior_dp38[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_dq8bu0pt[] = {
    { 1, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 2, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "mode_l1", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_dq8bu0pt_mode_l1_dp18, sizeof(kEnum__TZE200_dq8bu0pt_mode_l1_dp18)/sizeof(kEnum__TZE200_dq8bu0pt_mode_l1_dp18[0]) },
    { 19, "mode_l2", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_dq8bu0pt_mode_l2_dp19, sizeof(kEnum__TZE200_dq8bu0pt_mode_l2_dp19)/sizeof(kEnum__TZE200_dq8bu0pt_mode_l2_dp19[0]) },
    { 20, "mode_l3", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_dq8bu0pt_mode_l3_dp20, sizeof(kEnum__TZE200_dq8bu0pt_mode_l3_dp20)/sizeof(kEnum__TZE200_dq8bu0pt_mode_l3_dp20[0]) },
    { 21, "mode_l4", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_dq8bu0pt_mode_l4_dp21, sizeof(kEnum__TZE200_dq8bu0pt_mode_l4_dp21)/sizeof(kEnum__TZE200_dq8bu0pt_mode_l4_dp21[0]) },
    { 24, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 25, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 26, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 27, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 30, "countdown_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 31, "countdown_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 32, "countdown_l3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 33, "countdown_l4", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 36, "backlight_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 37, "indicator_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_dq8bu0pt_indicator_status_dp37, sizeof(kEnum__TZE200_dq8bu0pt_indicator_status_dp37)/sizeof(kEnum__TZE200_dq8bu0pt_indicator_status_dp37[0]) },
    { 38, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_dq8bu0pt_power_on_behavior_dp38, sizeof(kEnum__TZE200_dq8bu0pt_power_on_behavior_dp38)/sizeof(kEnum__TZE200_dq8bu0pt_power_on_behavior_dp38[0]) },
    { 103, "induction_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 105, "momentary_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "momentary_2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "momentary_3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 108, "momentary_4", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_dq8bu0pt{ kEntries__TZE200_dq8bu0pt, 24 };
constexpr FzConverter kFzDp__TZE200_dq8bu0pt{
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
    .user_config       = &kMap__TZE200_dq8bu0pt,
};
const FzConverter* const kFz__TZE200_dq8bu0pt[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_dq8bu0pt,
};
constexpr TzConverter kTzDp__TZE200_dq8bu0pt{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_dq8bu0pt,
};
const TzConverter* const kTz__TZE200_dq8bu0pt[] = { &kTzDp__TZE200_dq8bu0pt };
constexpr const char* kM__TZE200_dq8bu0pt[] = { "TS0601" };
constexpr const char* kN__TZE200_dq8bu0pt[] = { "_TZE200_dq8bu0pt" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_dq8bu0pt{
    .zigbee_models=kM__TZE200_dq8bu0pt,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_dq8bu0pt,.manufacturer_names_count=1,
    .model="TS0601__TZE200_dq8bu0pt",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_dq8bu0pt,
    .from_zigbee_count=sizeof(kFz__TZE200_dq8bu0pt)/sizeof(kFz__TZE200_dq8bu0pt[0]),
    .to_zigbee=kTz__TZE200_dq8bu0pt,
    .to_zigbee_count=sizeof(kTz__TZE200_dq8bu0pt)/sizeof(kTz__TZE200_dq8bu0pt[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
