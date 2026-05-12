// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_mexisfik).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_mexisfik[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_mexisfik{ kEntries__TZE200_mexisfik, 4 };
constexpr FzConverter kFzDp__TZE200_mexisfik{
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
    .user_config       = &kMap__TZE200_mexisfik,
};
const FzConverter* const kFz__TZE200_mexisfik[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_mexisfik,
};
constexpr TzConverter kTzDp__TZE200_mexisfik{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_mexisfik,
};
const TzConverter* const kTz__TZE200_mexisfik[] = { &kTzDp__TZE200_mexisfik };
constexpr const char* kM__TZE200_mexisfik[] = { "TS0601" };
constexpr const char* kN__TZE200_mexisfik[] = { "_TZE200_mexisfik" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_mexisfik{
    .zigbee_models=kM__TZE200_mexisfik,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_mexisfik,.manufacturer_names_count=1,
    .model="TS0601__TZE200_mexisfik",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_mexisfik,
    .from_zigbee_count=sizeof(kFz__TZE200_mexisfik)/sizeof(kFz__TZE200_mexisfik[0]),
    .to_zigbee=kTz__TZE200_mexisfik,
    .to_zigbee_count=sizeof(kTz__TZE200_mexisfik)/sizeof(kTz__TZE200_mexisfik[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
