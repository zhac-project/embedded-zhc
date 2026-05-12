// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_bw4ayyeh).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_bw4ayyeh_motion_state_dp11[] = {
    { 0, "none" },
    { 1, "static" },
    { 2, "small" },
    { 3, "large" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_bw4ayyeh[] = {
    { 1, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "distance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 11, "motion_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_bw4ayyeh_motion_state_dp11, sizeof(kEnum__TZE284_bw4ayyeh_motion_state_dp11)/sizeof(kEnum__TZE284_bw4ayyeh_motion_state_dp11[0]) },
    { 12, "fading_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 15, "motion_detection_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "static_detection_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 20, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 81, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "init", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "motion_detection_mode", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_bw4ayyeh{ kEntries__TZE284_bw4ayyeh, 10 };
constexpr FzConverter kFzDp__TZE284_bw4ayyeh{
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
    .user_config       = &kMap__TZE284_bw4ayyeh,
};
const FzConverter* const kFz__TZE284_bw4ayyeh[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_bw4ayyeh,
};
constexpr TzConverter kTzDp__TZE284_bw4ayyeh{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_bw4ayyeh,
};
const TzConverter* const kTz__TZE284_bw4ayyeh[] = { &kTzDp__TZE284_bw4ayyeh };
constexpr const char* kM__TZE284_bw4ayyeh[] = { "TS0601" };
constexpr const char* kN__TZE284_bw4ayyeh[] = { "_TZE284_bw4ayyeh" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_bw4ayyeh{
    .zigbee_models=kM__TZE284_bw4ayyeh,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_bw4ayyeh,.manufacturer_names_count=1,
    .model="TS0601__TZE284_bw4ayyeh",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_bw4ayyeh,
    .from_zigbee_count=sizeof(kFz__TZE284_bw4ayyeh)/sizeof(kFz__TZE284_bw4ayyeh[0]),
    .to_zigbee=kTz__TZE284_bw4ayyeh,
    .to_zigbee_count=sizeof(kTz__TZE284_bw4ayyeh)/sizeof(kTz__TZE284_bw4ayyeh[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
