// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_muvkrjr5).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_muvkrjr5[] = {
    { 1, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 13, "detection_range", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 16, "radar_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 19, "target_distance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "indicator", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 103, "fading_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_muvkrjr5{ kEntries__TZE204_muvkrjr5, 6 };
constexpr FzConverter kFzDp__TZE204_muvkrjr5{
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
    .user_config       = &kMap__TZE204_muvkrjr5,
};
const FzConverter* const kFz__TZE204_muvkrjr5[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_muvkrjr5,
};
constexpr TzConverter kTzDp__TZE204_muvkrjr5{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_muvkrjr5,
};
const TzConverter* const kTz__TZE204_muvkrjr5[] = { &kTzDp__TZE204_muvkrjr5 };
constexpr const char* kM__TZE204_muvkrjr5[] = { "TS0601" };
constexpr const char* kN__TZE204_muvkrjr5[] = { "_TZE204_muvkrjr5" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_muvkrjr5{
    .zigbee_models=kM__TZE204_muvkrjr5,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_muvkrjr5,.manufacturer_names_count=1,
    .model="TS0601__TZE204_muvkrjr5",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_muvkrjr5,
    .from_zigbee_count=sizeof(kFz__TZE204_muvkrjr5)/sizeof(kFz__TZE204_muvkrjr5[0]),
    .to_zigbee=kTz__TZE204_muvkrjr5,
    .to_zigbee_count=sizeof(kTz__TZE204_muvkrjr5)/sizeof(kTz__TZE204_muvkrjr5[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
