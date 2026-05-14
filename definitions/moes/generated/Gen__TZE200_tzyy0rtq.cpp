// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_tzyy0rtq).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_tzyy0rtq_mode_l1_dp18[] = {
    { 0, "switch_1" },
    { 1, "scene_1" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_tzyy0rtq_mode_l2_dp19[] = {
    { 0, "switch_2" },
    { 1, "scene_2" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_tzyy0rtq_indicator_status_dp37[] = {
    { 0, "off" },
    { 1, "relay" },
    { 2, "invert" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_tzyy0rtq_power_on_behavior_dp38[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_tzyy0rtq[] = {
    { 1, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 2, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "mode_l1", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_tzyy0rtq_mode_l1_dp18, sizeof(kEnum__TZE200_tzyy0rtq_mode_l1_dp18)/sizeof(kEnum__TZE200_tzyy0rtq_mode_l1_dp18[0]) },
    { 19, "mode_l2", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_tzyy0rtq_mode_l2_dp19, sizeof(kEnum__TZE200_tzyy0rtq_mode_l2_dp19)/sizeof(kEnum__TZE200_tzyy0rtq_mode_l2_dp19[0]) },
    { 24, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 25, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 30, "countdown_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 31, "countdown_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 37, "indicator_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_tzyy0rtq_indicator_status_dp37, sizeof(kEnum__TZE200_tzyy0rtq_indicator_status_dp37)/sizeof(kEnum__TZE200_tzyy0rtq_indicator_status_dp37[0]) },
    { 36, "backlight_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 38, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_tzyy0rtq_power_on_behavior_dp38, sizeof(kEnum__TZE200_tzyy0rtq_power_on_behavior_dp38)/sizeof(kEnum__TZE200_tzyy0rtq_power_on_behavior_dp38[0]) },
    { 103, "induction_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 105, "momentary_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "momentary_2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_tzyy0rtq{ kEntries__TZE200_tzyy0rtq, 14 };
constexpr FzConverter kFzDp__TZE200_tzyy0rtq{
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
    .user_config       = &kMap__TZE200_tzyy0rtq,
};
const FzConverter* const kFz__TZE200_tzyy0rtq[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_tzyy0rtq,
};
constexpr TzConverter kTzDp__TZE200_tzyy0rtq{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_tzyy0rtq,
};
const TzConverter* const kTz__TZE200_tzyy0rtq[] = { &kTzDp__TZE200_tzyy0rtq };
constexpr const char* kM__TZE200_tzyy0rtq[] = { "TS0601" };
constexpr const char* kN__TZE200_tzyy0rtq[] = { "_TZE200_tzyy0rtq" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_tzyy0rtq{
    .zigbee_models=kM__TZE200_tzyy0rtq,.zigbee_models_count=sizeof(kM__TZE200_tzyy0rtq)/sizeof(kM__TZE200_tzyy0rtq[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_tzyy0rtq,.manufacturer_names_count=1,
    .model="TS0601__TZE200_tzyy0rtq",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_tzyy0rtq,
    .from_zigbee_count=sizeof(kFz__TZE200_tzyy0rtq)/sizeof(kFz__TZE200_tzyy0rtq[0]),
    .to_zigbee=kTz__TZE200_tzyy0rtq,
    .to_zigbee_count=sizeof(kTz__TZE200_tzyy0rtq)/sizeof(kTz__TZE200_tzyy0rtq[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
