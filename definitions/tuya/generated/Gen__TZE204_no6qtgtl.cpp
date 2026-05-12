// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_no6qtgtl).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_no6qtgtl[] = {
    { 3, "near_detection", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 4, "far_detection", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 9, "target_distance_closest", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 101, "static_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "motion_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_no6qtgtl{ kEntries__TZE204_no6qtgtl, 5 };
constexpr FzConverter kFzDp__TZE204_no6qtgtl{
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
    .user_config       = &kMap__TZE204_no6qtgtl,
};
const FzConverter* const kFz__TZE204_no6qtgtl[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_no6qtgtl,
};
constexpr TzConverter kTzDp__TZE204_no6qtgtl{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_no6qtgtl,
};
const TzConverter* const kTz__TZE204_no6qtgtl[] = { &kTzDp__TZE204_no6qtgtl };
constexpr const char* kM__TZE204_no6qtgtl[] = { "TS0601" };
constexpr const char* kN__TZE204_no6qtgtl[] = { "_TZE204_no6qtgtl" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_no6qtgtl{
    .zigbee_models=kM__TZE204_no6qtgtl,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_no6qtgtl,.manufacturer_names_count=1,
    .model="TS0601__TZE204_no6qtgtl",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_no6qtgtl,
    .from_zigbee_count=sizeof(kFz__TZE204_no6qtgtl)/sizeof(kFz__TZE204_no6qtgtl[0]),
    .to_zigbee=kTz__TZE204_no6qtgtl,
    .to_zigbee_count=sizeof(kTz__TZE204_no6qtgtl)/sizeof(kTz__TZE204_no6qtgtl[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
