// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_z5jz7wpo).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_z5jz7wpo_power_on_behavior_dp11[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "restore" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_z5jz7wpo_light_mode_dp12[] = {
    { 0, "none" },
    { 1, "relay" },
    { 2, "pos" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_z5jz7wpo[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 11, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_z5jz7wpo_power_on_behavior_dp11, sizeof(kEnum__TZE284_z5jz7wpo_power_on_behavior_dp11)/sizeof(kEnum__TZE284_z5jz7wpo_power_on_behavior_dp11[0]) },
    { 12, "light_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_z5jz7wpo_light_mode_dp12, sizeof(kEnum__TZE284_z5jz7wpo_light_mode_dp12)/sizeof(kEnum__TZE284_z5jz7wpo_light_mode_dp12[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_z5jz7wpo{ kEntries__TZE284_z5jz7wpo, 3 };
constexpr FzConverter kFzDp__TZE284_z5jz7wpo{
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
    .user_config       = &kMap__TZE284_z5jz7wpo,
};
const FzConverter* const kFz__TZE284_z5jz7wpo[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_z5jz7wpo,
};
constexpr TzConverter kTzDp__TZE284_z5jz7wpo{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_z5jz7wpo,
};
const TzConverter* const kTz__TZE284_z5jz7wpo[] = { &kTzDp__TZE284_z5jz7wpo };
constexpr const char* kM__TZE284_z5jz7wpo[] = { "TS0601" };
constexpr const char* kN__TZE284_z5jz7wpo[] = { "_TZE284_z5jz7wpo" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_z5jz7wpo{
    .zigbee_models=kM__TZE284_z5jz7wpo,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_z5jz7wpo,.manufacturer_names_count=1,
    .model="TS0601__TZE284_z5jz7wpo",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_z5jz7wpo,
    .from_zigbee_count=sizeof(kFz__TZE284_z5jz7wpo)/sizeof(kFz__TZE284_z5jz7wpo[0]),
    .to_zigbee=kTz__TZE284_z5jz7wpo,
    .to_zigbee_count=sizeof(kTz__TZE284_z5jz7wpo)/sizeof(kTz__TZE284_z5jz7wpo[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
