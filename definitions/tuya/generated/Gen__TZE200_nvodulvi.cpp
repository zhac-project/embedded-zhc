// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_nvodulvi).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_power_on_behavior_l1_dp29[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_power_on_behavior_l2_dp30[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_power_on_behavior_l3_dp31[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_power_on_behavior_l4_dp32[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_power_on_behavior_l5_dp33[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_power_on_behavior_l6_dp34[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_switch_mode_l1_dp114[] = {
    { 0, "switch" },
    { 1, "scene" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_switch_mode_l2_dp115[] = {
    { 0, "switch" },
    { 1, "scene" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_switch_mode_l3_dp116[] = {
    { 0, "switch" },
    { 1, "scene" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_switch_mode_l4_dp117[] = {
    { 0, "switch" },
    { 1, "scene" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_switch_mode_l5_dp118[] = {
    { 0, "switch" },
    { 1, "scene" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_switch_mode_l6_dp119[] = {
    { 0, "switch" },
    { 1, "scene" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_switch_mode_l7_dp120[] = {
    { 0, "switch" },
    { 1, "scene" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_switch_mode_l8_dp121[] = {
    { 0, "switch" },
    { 1, "scene" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_light_mode_l1_dp122[] = {
    { 0, "normal" },
    { 1, "on" },
    { 2, "off" },
    { 3, "flash" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_light_mode_l2_dp123[] = {
    { 0, "normal" },
    { 1, "on" },
    { 2, "off" },
    { 3, "flash" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_light_mode_l3_dp124[] = {
    { 0, "normal" },
    { 1, "on" },
    { 2, "off" },
    { 3, "flash" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_light_mode_l4_dp125[] = {
    { 0, "normal" },
    { 1, "on" },
    { 2, "off" },
    { 3, "flash" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_light_mode_l5_dp126[] = {
    { 0, "normal" },
    { 1, "on" },
    { 2, "off" },
    { 3, "flash" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_light_mode_l6_dp127[] = {
    { 0, "normal" },
    { 1, "on" },
    { 2, "off" },
    { 3, "flash" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_light_mode_l7_dp128[] = {
    { 0, "normal" },
    { 1, "on" },
    { 2, "off" },
    { 3, "flash" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_light_mode_l8_dp129[] = {
    { 0, "normal" },
    { 1, "on" },
    { 2, "off" },
    { 3, "flash" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_power_on_behavior_l7_dp130[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nvodulvi_power_on_behavior_l8_dp131[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_nvodulvi[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 5, "state_l5", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 6, "state_l6", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 16, "backlight_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 29, "power_on_behavior_l1", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_power_on_behavior_l1_dp29, sizeof(kEnum__TZE200_nvodulvi_power_on_behavior_l1_dp29)/sizeof(kEnum__TZE200_nvodulvi_power_on_behavior_l1_dp29[0]) },
    { 30, "power_on_behavior_l2", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_power_on_behavior_l2_dp30, sizeof(kEnum__TZE200_nvodulvi_power_on_behavior_l2_dp30)/sizeof(kEnum__TZE200_nvodulvi_power_on_behavior_l2_dp30[0]) },
    { 31, "power_on_behavior_l3", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_power_on_behavior_l3_dp31, sizeof(kEnum__TZE200_nvodulvi_power_on_behavior_l3_dp31)/sizeof(kEnum__TZE200_nvodulvi_power_on_behavior_l3_dp31[0]) },
    { 32, "power_on_behavior_l4", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_power_on_behavior_l4_dp32, sizeof(kEnum__TZE200_nvodulvi_power_on_behavior_l4_dp32)/sizeof(kEnum__TZE200_nvodulvi_power_on_behavior_l4_dp32[0]) },
    { 33, "power_on_behavior_l5", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_power_on_behavior_l5_dp33, sizeof(kEnum__TZE200_nvodulvi_power_on_behavior_l5_dp33)/sizeof(kEnum__TZE200_nvodulvi_power_on_behavior_l5_dp33[0]) },
    { 34, "power_on_behavior_l6", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_power_on_behavior_l6_dp34, sizeof(kEnum__TZE200_nvodulvi_power_on_behavior_l6_dp34)/sizeof(kEnum__TZE200_nvodulvi_power_on_behavior_l6_dp34[0]) },
    { 101, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "presence", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "delay", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 108, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 110, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 112, "state_l7", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 113, "state_l8", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 114, "switch_mode_l1", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_switch_mode_l1_dp114, sizeof(kEnum__TZE200_nvodulvi_switch_mode_l1_dp114)/sizeof(kEnum__TZE200_nvodulvi_switch_mode_l1_dp114[0]) },
    { 115, "switch_mode_l2", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_switch_mode_l2_dp115, sizeof(kEnum__TZE200_nvodulvi_switch_mode_l2_dp115)/sizeof(kEnum__TZE200_nvodulvi_switch_mode_l2_dp115[0]) },
    { 116, "switch_mode_l3", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_switch_mode_l3_dp116, sizeof(kEnum__TZE200_nvodulvi_switch_mode_l3_dp116)/sizeof(kEnum__TZE200_nvodulvi_switch_mode_l3_dp116[0]) },
    { 117, "switch_mode_l4", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_switch_mode_l4_dp117, sizeof(kEnum__TZE200_nvodulvi_switch_mode_l4_dp117)/sizeof(kEnum__TZE200_nvodulvi_switch_mode_l4_dp117[0]) },
    { 118, "switch_mode_l5", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_switch_mode_l5_dp118, sizeof(kEnum__TZE200_nvodulvi_switch_mode_l5_dp118)/sizeof(kEnum__TZE200_nvodulvi_switch_mode_l5_dp118[0]) },
    { 119, "switch_mode_l6", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_switch_mode_l6_dp119, sizeof(kEnum__TZE200_nvodulvi_switch_mode_l6_dp119)/sizeof(kEnum__TZE200_nvodulvi_switch_mode_l6_dp119[0]) },
    { 120, "switch_mode_l7", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_switch_mode_l7_dp120, sizeof(kEnum__TZE200_nvodulvi_switch_mode_l7_dp120)/sizeof(kEnum__TZE200_nvodulvi_switch_mode_l7_dp120[0]) },
    { 121, "switch_mode_l8", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_switch_mode_l8_dp121, sizeof(kEnum__TZE200_nvodulvi_switch_mode_l8_dp121)/sizeof(kEnum__TZE200_nvodulvi_switch_mode_l8_dp121[0]) },
    { 122, "light_mode_l1", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_light_mode_l1_dp122, sizeof(kEnum__TZE200_nvodulvi_light_mode_l1_dp122)/sizeof(kEnum__TZE200_nvodulvi_light_mode_l1_dp122[0]) },
    { 123, "light_mode_l2", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_light_mode_l2_dp123, sizeof(kEnum__TZE200_nvodulvi_light_mode_l2_dp123)/sizeof(kEnum__TZE200_nvodulvi_light_mode_l2_dp123[0]) },
    { 124, "light_mode_l3", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_light_mode_l3_dp124, sizeof(kEnum__TZE200_nvodulvi_light_mode_l3_dp124)/sizeof(kEnum__TZE200_nvodulvi_light_mode_l3_dp124[0]) },
    { 125, "light_mode_l4", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_light_mode_l4_dp125, sizeof(kEnum__TZE200_nvodulvi_light_mode_l4_dp125)/sizeof(kEnum__TZE200_nvodulvi_light_mode_l4_dp125[0]) },
    { 126, "light_mode_l5", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_light_mode_l5_dp126, sizeof(kEnum__TZE200_nvodulvi_light_mode_l5_dp126)/sizeof(kEnum__TZE200_nvodulvi_light_mode_l5_dp126[0]) },
    { 127, "light_mode_l6", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_light_mode_l6_dp127, sizeof(kEnum__TZE200_nvodulvi_light_mode_l6_dp127)/sizeof(kEnum__TZE200_nvodulvi_light_mode_l6_dp127[0]) },
    { 128, "light_mode_l7", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_light_mode_l7_dp128, sizeof(kEnum__TZE200_nvodulvi_light_mode_l7_dp128)/sizeof(kEnum__TZE200_nvodulvi_light_mode_l7_dp128[0]) },
    { 129, "light_mode_l8", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_light_mode_l8_dp129, sizeof(kEnum__TZE200_nvodulvi_light_mode_l8_dp129)/sizeof(kEnum__TZE200_nvodulvi_light_mode_l8_dp129[0]) },
    { 130, "power_on_behavior_l7", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_power_on_behavior_l7_dp130, sizeof(kEnum__TZE200_nvodulvi_power_on_behavior_l7_dp130)/sizeof(kEnum__TZE200_nvodulvi_power_on_behavior_l7_dp130[0]) },
    { 131, "power_on_behavior_l8", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nvodulvi_power_on_behavior_l8_dp131, sizeof(kEnum__TZE200_nvodulvi_power_on_behavior_l8_dp131)/sizeof(kEnum__TZE200_nvodulvi_power_on_behavior_l8_dp131[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_nvodulvi{ kEntries__TZE200_nvodulvi, 43 };
constexpr FzConverter kFzDp__TZE200_nvodulvi{
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
    .user_config       = &kMap__TZE200_nvodulvi,
};
const FzConverter* const kFz__TZE200_nvodulvi[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_nvodulvi,
};
constexpr TzConverter kTzDp__TZE200_nvodulvi{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_nvodulvi,
};
const TzConverter* const kTz__TZE200_nvodulvi[] = { &kTzDp__TZE200_nvodulvi };
constexpr const char* kM__TZE200_nvodulvi[] = { "TS0601" };
constexpr const char* kN__TZE200_nvodulvi[] = { "_TZE200_nvodulvi" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_nvodulvi{
    .zigbee_models=kM__TZE200_nvodulvi,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_nvodulvi,.manufacturer_names_count=1,
    .model="TS0601__TZE200_nvodulvi",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_nvodulvi,
    .from_zigbee_count=sizeof(kFz__TZE200_nvodulvi)/sizeof(kFz__TZE200_nvodulvi[0]),
    .to_zigbee=kTz__TZE200_nvodulvi,
    .to_zigbee_count=sizeof(kTz__TZE200_nvodulvi)/sizeof(kTz__TZE200_nvodulvi[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
