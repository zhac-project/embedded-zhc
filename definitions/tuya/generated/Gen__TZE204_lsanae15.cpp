// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_lsanae15).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_lsanae15[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 16, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 18, "meter_id", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 20, "clear_fault", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_lsanae15{ kEntries__TZE204_lsanae15, 4 };
constexpr FzConverter kFzDp__TZE204_lsanae15{
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
    .user_config       = &kMap__TZE204_lsanae15,
};
const FzConverter* const kFz__TZE204_lsanae15[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_lsanae15,
};
constexpr TzConverter kTzDp__TZE204_lsanae15{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_lsanae15,
};
const TzConverter* const kTz__TZE204_lsanae15[] = { &kTzDp__TZE204_lsanae15 };
constexpr const char* kM__TZE204_lsanae15[] = { "TS0601" };
constexpr const char* kN__TZE204_lsanae15[] = { "_TZE204_lsanae15" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_lsanae15{
    .zigbee_models=kM__TZE204_lsanae15,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_lsanae15,.manufacturer_names_count=1,
    .model="TS0601__TZE204_lsanae15",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_lsanae15,
    .from_zigbee_count=sizeof(kFz__TZE204_lsanae15)/sizeof(kFz__TZE204_lsanae15[0]),
    .to_zigbee=kTz__TZE204_lsanae15,
    .to_zigbee_count=sizeof(kTz__TZE204_lsanae15)/sizeof(kTz__TZE204_lsanae15[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
