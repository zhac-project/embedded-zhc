// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_bmdsp6bs).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_bmdsp6bs[] = {
    { 1, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "fading_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 110, "keep_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 114, "trigger_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 182, "target_distance", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_bmdsp6bs{ kEntries__TZE204_bmdsp6bs, 6 };
constexpr FzConverter kFzDp__TZE204_bmdsp6bs{
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
    .user_config       = &kMap__TZE204_bmdsp6bs,
};
const FzConverter* const kFz__TZE204_bmdsp6bs[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_bmdsp6bs,
};
constexpr TzConverter kTzDp__TZE204_bmdsp6bs{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_bmdsp6bs,
};
const TzConverter* const kTz__TZE204_bmdsp6bs[] = { &kTzDp__TZE204_bmdsp6bs };
constexpr const char* kM__TZE204_bmdsp6bs[] = { "TS0601" };
constexpr const char* kN__TZE204_bmdsp6bs[] = { "_TZE204_bmdsp6bs" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_bmdsp6bs{
    .zigbee_models=kM__TZE204_bmdsp6bs,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_bmdsp6bs,.manufacturer_names_count=1,
    .model="TS0601__TZE204_bmdsp6bs",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_bmdsp6bs,
    .from_zigbee_count=sizeof(kFz__TZE204_bmdsp6bs)/sizeof(kFz__TZE204_bmdsp6bs[0]),
    .to_zigbee=kTz__TZE204_bmdsp6bs,
    .to_zigbee_count=sizeof(kTz__TZE204_bmdsp6bs)/sizeof(kTz__TZE204_bmdsp6bs[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
