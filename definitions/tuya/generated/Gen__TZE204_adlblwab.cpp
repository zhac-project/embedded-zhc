// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_adlblwab).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_adlblwab_power_on_behavior_dp27[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_adlblwab[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 5, "state_l5", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 6, "state_l6", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 7, "state_l7", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 8, "state_l8", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 9, "countdown_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 10, "countdown_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 11, "countdown_l3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 12, "countdown_l4", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 13, "countdown_l5", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "countdown_l6", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 15, "countdown_l7", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "countdown_l8", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 27, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_adlblwab_power_on_behavior_dp27, sizeof(kEnum__TZE204_adlblwab_power_on_behavior_dp27)/sizeof(kEnum__TZE204_adlblwab_power_on_behavior_dp27[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_adlblwab{ kEntries__TZE204_adlblwab, 17 };
constexpr FzConverter kFzDp__TZE204_adlblwab{
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
    .user_config       = &kMap__TZE204_adlblwab,
};
const FzConverter* const kFz__TZE204_adlblwab[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_adlblwab,
};
constexpr TzConverter kTzDp__TZE204_adlblwab{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_adlblwab,
};
const TzConverter* const kTz__TZE204_adlblwab[] = { &kTzDp__TZE204_adlblwab };
constexpr const char* kM__TZE204_adlblwab[] = { "TS0601" };
constexpr const char* kN__TZE204_adlblwab[] = { "_TZE204_adlblwab" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_adlblwab{
    .zigbee_models=kM__TZE204_adlblwab,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_adlblwab,.manufacturer_names_count=1,
    .model="TS0601__TZE204_adlblwab",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_adlblwab,
    .from_zigbee_count=sizeof(kFz__TZE204_adlblwab)/sizeof(kFz__TZE204_adlblwab[0]),
    .to_zigbee=kTz__TZE204_adlblwab,
    .to_zigbee_count=sizeof(kTz__TZE204_adlblwab)/sizeof(kTz__TZE204_adlblwab[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
