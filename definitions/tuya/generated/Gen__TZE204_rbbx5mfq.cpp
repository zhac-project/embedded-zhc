// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_rbbx5mfq).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_rbbx5mfq[] = {
    { 2, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 6, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 7, "humidity", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_rbbx5mfq{ kEntries__TZE204_rbbx5mfq, 3 };
constexpr FzConverter kFzDp__TZE204_rbbx5mfq{
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
    .user_config       = &kMap__TZE204_rbbx5mfq,
};
const FzConverter* const kFz__TZE204_rbbx5mfq[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_rbbx5mfq,
};
constexpr TzConverter kTzDp__TZE204_rbbx5mfq{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_rbbx5mfq,
};
const TzConverter* const kTz__TZE204_rbbx5mfq[] = { &kTzDp__TZE204_rbbx5mfq };
constexpr const char* kM__TZE204_rbbx5mfq[] = { "TS0601" };
constexpr const char* kN__TZE204_rbbx5mfq[] = { "_TZE204_rbbx5mfq" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_rbbx5mfq{
    .zigbee_models=kM__TZE204_rbbx5mfq,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_rbbx5mfq,.manufacturer_names_count=1,
    .model="TS0601__TZE204_rbbx5mfq",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_rbbx5mfq,
    .from_zigbee_count=sizeof(kFz__TZE204_rbbx5mfq)/sizeof(kFz__TZE204_rbbx5mfq[0]),
    .to_zigbee=kTz__TZE204_rbbx5mfq,
    .to_zigbee_count=sizeof(kTz__TZE204_rbbx5mfq)/sizeof(kTz__TZE204_rbbx5mfq[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
