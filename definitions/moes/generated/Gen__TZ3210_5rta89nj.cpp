// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZ3210_5rta89nj).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZ3210_5rta89nj[] = {
    { 104, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "charging", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZ3210_5rta89nj{ kEntries__TZ3210_5rta89nj, 2 };
constexpr FzConverter kFzDp__TZ3210_5rta89nj{
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
    .user_config       = &kMap__TZ3210_5rta89nj,
};
const FzConverter* const kFz__TZ3210_5rta89nj[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZ3210_5rta89nj,
};
constexpr TzConverter kTzDp__TZ3210_5rta89nj{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZ3210_5rta89nj,
};
const TzConverter* const kTz__TZ3210_5rta89nj[] = { &kTzDp__TZ3210_5rta89nj };
constexpr const char* kM__TZ3210_5rta89nj[] = { "TS0601" };
constexpr const char* kN__TZ3210_5rta89nj[] = { "_TZ3210_5rta89nj" };
}  // namespace
extern const PreparedDefinition kDefGen__TZ3210_5rta89nj{
    .zigbee_models=kM__TZ3210_5rta89nj,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZ3210_5rta89nj,.manufacturer_names_count=1,
    .model="TS0601__TZ3210_5rta89nj",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZ3210_5rta89nj,
    .from_zigbee_count=sizeof(kFz__TZ3210_5rta89nj)/sizeof(kFz__TZ3210_5rta89nj[0]),
    .to_zigbee=kTz__TZ3210_5rta89nj,
    .to_zigbee_count=sizeof(kTz__TZ3210_5rta89nj)/sizeof(kTz__TZ3210_5rta89nj[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
