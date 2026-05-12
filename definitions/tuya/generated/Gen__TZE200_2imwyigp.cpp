// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_2imwyigp).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_2imwyigp[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_2imwyigp{ kEntries__TZE200_2imwyigp, 3 };
constexpr FzConverter kFzDp__TZE200_2imwyigp{
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
    .user_config       = &kMap__TZE200_2imwyigp,
};
const FzConverter* const kFz__TZE200_2imwyigp[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_2imwyigp,
};
constexpr TzConverter kTzDp__TZE200_2imwyigp{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_2imwyigp,
};
const TzConverter* const kTz__TZE200_2imwyigp[] = { &kTzDp__TZE200_2imwyigp };
constexpr const char* kM__TZE200_2imwyigp[] = { "TS0601" };
constexpr const char* kN__TZE200_2imwyigp[] = { "_TZE200_2imwyigp" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_2imwyigp{
    .zigbee_models=kM__TZE200_2imwyigp,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_2imwyigp,.manufacturer_names_count=1,
    .model="TS0601__TZE200_2imwyigp",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_2imwyigp,
    .from_zigbee_count=sizeof(kFz__TZE200_2imwyigp)/sizeof(kFz__TZE200_2imwyigp[0]),
    .to_zigbee=kTz__TZE200_2imwyigp,
    .to_zigbee_count=sizeof(kTz__TZE200_2imwyigp)/sizeof(kTz__TZE200_2imwyigp[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
