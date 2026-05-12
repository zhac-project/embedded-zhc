// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_yrwmnya3).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_yrwmnya3_switch_mode_l1_dp18[] = {
    { 0, "switch" },
    { 1, "scene" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_yrwmnya3_switch_mode_l2_dp19[] = {
    { 0, "switch" },
    { 1, "scene" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_yrwmnya3_switch_mode_l3_dp20[] = {
    { 0, "switch" },
    { 1, "scene" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_yrwmnya3_switch_mode_l4_dp21[] = {
    { 0, "switch" },
    { 1, "scene" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_yrwmnya3_power_on_behavior_l0_dp38[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_yrwmnya3_power_on_behavior_l1_dp39[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_yrwmnya3_power_on_behavior_l2_dp40[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_yrwmnya3_power_on_behavior_l3_dp41[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_yrwmnya3_power_on_behavior_l4_dp42[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_yrwmnya3[] = {
    { 1, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 2, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 6, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "switch_mode_l1", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_yrwmnya3_switch_mode_l1_dp18, sizeof(kEnum__TZE284_yrwmnya3_switch_mode_l1_dp18)/sizeof(kEnum__TZE284_yrwmnya3_switch_mode_l1_dp18[0]) },
    { 19, "switch_mode_l2", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_yrwmnya3_switch_mode_l2_dp19, sizeof(kEnum__TZE284_yrwmnya3_switch_mode_l2_dp19)/sizeof(kEnum__TZE284_yrwmnya3_switch_mode_l2_dp19[0]) },
    { 20, "switch_mode_l3", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_yrwmnya3_switch_mode_l3_dp20, sizeof(kEnum__TZE284_yrwmnya3_switch_mode_l3_dp20)/sizeof(kEnum__TZE284_yrwmnya3_switch_mode_l3_dp20[0]) },
    { 21, "switch_mode_l4", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_yrwmnya3_switch_mode_l4_dp21, sizeof(kEnum__TZE284_yrwmnya3_switch_mode_l4_dp21)/sizeof(kEnum__TZE284_yrwmnya3_switch_mode_l4_dp21[0]) },
    { 24, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 25, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 26, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 27, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 36, "backlight_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 38, "power_on_behavior_l0", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_yrwmnya3_power_on_behavior_l0_dp38, sizeof(kEnum__TZE284_yrwmnya3_power_on_behavior_l0_dp38)/sizeof(kEnum__TZE284_yrwmnya3_power_on_behavior_l0_dp38[0]) },
    { 39, "power_on_behavior_l1", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_yrwmnya3_power_on_behavior_l1_dp39, sizeof(kEnum__TZE284_yrwmnya3_power_on_behavior_l1_dp39)/sizeof(kEnum__TZE284_yrwmnya3_power_on_behavior_l1_dp39[0]) },
    { 40, "power_on_behavior_l2", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_yrwmnya3_power_on_behavior_l2_dp40, sizeof(kEnum__TZE284_yrwmnya3_power_on_behavior_l2_dp40)/sizeof(kEnum__TZE284_yrwmnya3_power_on_behavior_l2_dp40[0]) },
    { 41, "power_on_behavior_l3", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_yrwmnya3_power_on_behavior_l3_dp41, sizeof(kEnum__TZE284_yrwmnya3_power_on_behavior_l3_dp41)/sizeof(kEnum__TZE284_yrwmnya3_power_on_behavior_l3_dp41[0]) },
    { 42, "power_on_behavior_l4", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_yrwmnya3_power_on_behavior_l4_dp42, sizeof(kEnum__TZE284_yrwmnya3_power_on_behavior_l4_dp42)/sizeof(kEnum__TZE284_yrwmnya3_power_on_behavior_l4_dp42[0]) },
    { 101, "presence", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "delay", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_yrwmnya3{ kEntries__TZE284_yrwmnya3, 25 };
constexpr FzConverter kFzDp__TZE284_yrwmnya3{
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
    .user_config       = &kMap__TZE284_yrwmnya3,
};
const FzConverter* const kFz__TZE284_yrwmnya3[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_yrwmnya3,
};
constexpr TzConverter kTzDp__TZE284_yrwmnya3{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_yrwmnya3,
};
const TzConverter* const kTz__TZE284_yrwmnya3[] = { &kTzDp__TZE284_yrwmnya3 };
constexpr const char* kM__TZE284_yrwmnya3[] = { "TS0601" };
constexpr const char* kN__TZE284_yrwmnya3[] = { "_TZE284_yrwmnya3" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_yrwmnya3{
    .zigbee_models=kM__TZE284_yrwmnya3,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_yrwmnya3,.manufacturer_names_count=1,
    .model="TS0601__TZE284_yrwmnya3",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_yrwmnya3,
    .from_zigbee_count=sizeof(kFz__TZE284_yrwmnya3)/sizeof(kFz__TZE284_yrwmnya3[0]),
    .to_zigbee=kTz__TZE284_yrwmnya3,
    .to_zigbee_count=sizeof(kTz__TZE284_yrwmnya3)/sizeof(kTz__TZE284_yrwmnya3[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
