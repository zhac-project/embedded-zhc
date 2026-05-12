// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_zbfmvj13).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_zbfmvj13[] = {
    { 1, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "fading_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "detection_distance", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 2, "static_detection_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "indicator", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 123, "motion_detection_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 121, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 122, "anti_interference", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_zbfmvj13{ kEntries__TZE200_zbfmvj13, 8 };
constexpr FzConverter kFzDp__TZE200_zbfmvj13{
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
    .user_config       = &kMap__TZE200_zbfmvj13,
};
const FzConverter* const kFz__TZE200_zbfmvj13[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_zbfmvj13,
};
constexpr TzConverter kTzDp__TZE200_zbfmvj13{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_zbfmvj13,
};
const TzConverter* const kTz__TZE200_zbfmvj13[] = { &kTzDp__TZE200_zbfmvj13 };
constexpr const char* kM__TZE200_zbfmvj13[] = { "TS0601" };
constexpr const char* kN__TZE200_zbfmvj13[] = { "_TZE200_zbfmvj13" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_zbfmvj13{
    .zigbee_models=kM__TZE200_zbfmvj13,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_zbfmvj13,.manufacturer_names_count=1,
    .model="TS0601__TZE200_zbfmvj13",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_zbfmvj13,
    .from_zigbee_count=sizeof(kFz__TZE200_zbfmvj13)/sizeof(kFz__TZE200_zbfmvj13[0]),
    .to_zigbee=kTz__TZE200_zbfmvj13,
    .to_zigbee_count=sizeof(kTz__TZE200_zbfmvj13)/sizeof(kTz__TZE200_zbfmvj13[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
