// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_kijxnb8q).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_kijxnb8q_power_outage_memory_dp112[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "restore" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_kijxnb8q_auto_on_dp113[] = {
    { 0, "off" },
    { 1, "all" },
    { 1, "ch1" },
    { 2, "ch2" },
    { 3, "ch3" },
    { 4, "ch1_2" },
    { 5, "ch2_3" },
    { 6, "ch1_3" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_kijxnb8q_auto_off_dp115[] = {
    { 0, "off" },
    { 1, "all" },
    { 1, "ch1" },
    { 2, "ch2" },
    { 3, "ch3" },
    { 4, "ch1_2" },
    { 5, "ch2_3" },
    { 6, "ch1_3" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_kijxnb8q_trigger_switch_dp108[] = {
    { 0, "ch1" },
    { 1, "ch2" },
    { 2, "ch3" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_kijxnb8q[] = {
    { 1, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "distance", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 101, "switch1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "switch2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 103, "switch3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 111, "backlight", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 114, "trigger_hold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 112, "power_outage_memory", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_kijxnb8q_power_outage_memory_dp112, sizeof(kEnum__TZE200_kijxnb8q_power_outage_memory_dp112)/sizeof(kEnum__TZE200_kijxnb8q_power_outage_memory_dp112[0]) },
    { 113, "auto_on", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_kijxnb8q_auto_on_dp113, sizeof(kEnum__TZE200_kijxnb8q_auto_on_dp113)/sizeof(kEnum__TZE200_kijxnb8q_auto_on_dp113[0]) },
    { 115, "auto_off", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_kijxnb8q_auto_off_dp115, sizeof(kEnum__TZE200_kijxnb8q_auto_off_dp115)/sizeof(kEnum__TZE200_kijxnb8q_auto_off_dp115[0]) },
    { 108, "trigger_switch", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_kijxnb8q_trigger_switch_dp108, sizeof(kEnum__TZE200_kijxnb8q_trigger_switch_dp108)/sizeof(kEnum__TZE200_kijxnb8q_trigger_switch_dp108[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_kijxnb8q{ kEntries__TZE200_kijxnb8q, 12 };
constexpr FzConverter kFzDp__TZE200_kijxnb8q{
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
    .user_config       = &kMap__TZE200_kijxnb8q,
};
const FzConverter* const kFz__TZE200_kijxnb8q[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_kijxnb8q,
};
constexpr TzConverter kTzDp__TZE200_kijxnb8q{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_kijxnb8q,
};
const TzConverter* const kTz__TZE200_kijxnb8q[] = { &kTzDp__TZE200_kijxnb8q };
constexpr const char* kM__TZE200_kijxnb8q[] = { "TS0601" };
constexpr const char* kN__TZE200_kijxnb8q[] = { "_TZE200_kijxnb8q" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_kijxnb8q{
    .zigbee_models=kM__TZE200_kijxnb8q,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_kijxnb8q,.manufacturer_names_count=1,
    .model="TS0601__TZE200_kijxnb8q",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_kijxnb8q,
    .from_zigbee_count=sizeof(kFz__TZE200_kijxnb8q)/sizeof(kFz__TZE200_kijxnb8q[0]),
    .to_zigbee=kTz__TZE200_kijxnb8q,
    .to_zigbee_count=sizeof(kTz__TZE200_kijxnb8q)/sizeof(kTz__TZE200_kijxnb8q[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
