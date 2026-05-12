// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_iik0pquw).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_iik0pquw[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_iik0pquw{ kEntries__TZE204_iik0pquw, 4 };
constexpr FzConverter kFzDp__TZE204_iik0pquw{
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
    .user_config       = &kMap__TZE204_iik0pquw,
};
const FzConverter* const kFz__TZE204_iik0pquw[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_iik0pquw,
};
constexpr TzConverter kTzDp__TZE204_iik0pquw{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_iik0pquw,
};
const TzConverter* const kTz__TZE204_iik0pquw[] = { &kTzDp__TZE204_iik0pquw };
constexpr const char* kM__TZE204_iik0pquw[] = { "TS0601" };
constexpr const char* kN__TZE204_iik0pquw[] = { "_TZE204_iik0pquw" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_iik0pquw{
    .zigbee_models=kM__TZE204_iik0pquw,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_iik0pquw,.manufacturer_names_count=1,
    .model="TS0601__TZE204_iik0pquw",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_iik0pquw,
    .from_zigbee_count=sizeof(kFz__TZE204_iik0pquw)/sizeof(kFz__TZE204_iik0pquw[0]),
    .to_zigbee=kTz__TZE204_iik0pquw,
    .to_zigbee_count=sizeof(kTz__TZE204_iik0pquw)/sizeof(kTz__TZE204_iik0pquw[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
