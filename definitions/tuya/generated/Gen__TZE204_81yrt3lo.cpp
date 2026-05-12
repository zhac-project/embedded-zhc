// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_81yrt3lo).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_81yrt3lo[] = {
    { 111, "ac_frequency", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 112, "voltage", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 129, "update_frequency", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_81yrt3lo{ kEntries__TZE204_81yrt3lo, 3 };
constexpr FzConverter kFzDp__TZE204_81yrt3lo{
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
    .user_config       = &kMap__TZE204_81yrt3lo,
};
const FzConverter* const kFz__TZE204_81yrt3lo[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_81yrt3lo,
};
constexpr TzConverter kTzDp__TZE204_81yrt3lo{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_81yrt3lo,
};
const TzConverter* const kTz__TZE204_81yrt3lo[] = { &kTzDp__TZE204_81yrt3lo };
constexpr const char* kM__TZE204_81yrt3lo[] = { "TS0601" };
constexpr const char* kN__TZE204_81yrt3lo[] = { "_TZE204_81yrt3lo" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_81yrt3lo{
    .zigbee_models=kM__TZE204_81yrt3lo,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_81yrt3lo,.manufacturer_names_count=1,
    .model="TS0601__TZE204_81yrt3lo",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_81yrt3lo,
    .from_zigbee_count=sizeof(kFz__TZE204_81yrt3lo)/sizeof(kFz__TZE204_81yrt3lo[0]),
    .to_zigbee=kTz__TZE204_81yrt3lo,
    .to_zigbee_count=sizeof(kTz__TZE204_81yrt3lo)/sizeof(kTz__TZE204_81yrt3lo[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
