// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_eaulras5).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_eaulras5[] = {
    { 104, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 112, "occupancy", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 9, "closest_target_distance", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 101, "movement_timeout", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "idle_timeout", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "illuminance", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 105, "far_movement_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 110, "near_movement_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "near_presence_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "far_presence_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "closest_detection_distance", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 4, "largest_movement_detection_distance", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 108, "largest_presence_detection_distance", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_eaulras5{ kEntries__TZE204_eaulras5, 13 };
constexpr FzConverter kFzDp__TZE204_eaulras5{
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
    .user_config       = &kMap__TZE204_eaulras5,
};
const FzConverter* const kFz__TZE204_eaulras5[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_eaulras5,
};
constexpr TzConverter kTzDp__TZE204_eaulras5{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_eaulras5,
};
const TzConverter* const kTz__TZE204_eaulras5[] = { &kTzDp__TZE204_eaulras5 };
constexpr const char* kM__TZE204_eaulras5[] = { "TS0601" };
constexpr const char* kN__TZE204_eaulras5[] = { "_TZE204_eaulras5" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_eaulras5{
    .zigbee_models=kM__TZE204_eaulras5,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_eaulras5,.manufacturer_names_count=1,
    .model="TS0601__TZE204_eaulras5",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_eaulras5,
    .from_zigbee_count=sizeof(kFz__TZE204_eaulras5)/sizeof(kFz__TZE204_eaulras5[0]),
    .to_zigbee=kTz__TZE204_eaulras5,
    .to_zigbee_count=sizeof(kTz__TZE204_eaulras5)/sizeof(kTz__TZE204_eaulras5[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
