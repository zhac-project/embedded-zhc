// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_8ply8mjj).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_8ply8mjj[] = {
    { 1, "vibration", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 101, "sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "buzzer_mute", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_8ply8mjj{ kEntries__TZE200_8ply8mjj, 3 };
constexpr FzConverter kFzDp__TZE200_8ply8mjj{
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
    .user_config       = &kMap__TZE200_8ply8mjj,
};
const FzConverter* const kFz__TZE200_8ply8mjj[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_8ply8mjj,
};
constexpr TzConverter kTzDp__TZE200_8ply8mjj{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_8ply8mjj,
};
const TzConverter* const kTz__TZE200_8ply8mjj[] = { &kTzDp__TZE200_8ply8mjj };
constexpr const char* kM__TZE200_8ply8mjj[] = { "TS0601" };
constexpr const char* kN__TZE200_8ply8mjj[] = { "_TZE200_8ply8mjj" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_8ply8mjj{
    .zigbee_models=kM__TZE200_8ply8mjj,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_8ply8mjj,.manufacturer_names_count=1,
    .model="TS0601__TZE200_8ply8mjj",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_8ply8mjj,
    .from_zigbee_count=sizeof(kFz__TZE200_8ply8mjj)/sizeof(kFz__TZE200_8ply8mjj[0]),
    .to_zigbee=kTz__TZE200_8ply8mjj,
    .to_zigbee_count=sizeof(kTz__TZE200_8ply8mjj)/sizeof(kTz__TZE200_8ply8mjj[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
