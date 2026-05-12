// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_iwyqtclw).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_iwyqtclw_mode_l1_dp18[] = {
    { 0, "switch" },
    { 1, "scene" },
    { 2, "smart_light" },
    { 3, "curtain" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_iwyqtclw_mode_l2_dp19[] = {
    { 0, "switch" },
    { 1, "scene" },
    { 2, "smart_light" },
    { 3, "curtain" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_iwyqtclw_mode_l3_dp20[] = {
    { 0, "switch" },
    { 1, "scene" },
    { 2, "smart_light" },
    { 3, "curtain" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_iwyqtclw_mode_l4_dp21[] = {
    { 0, "switch" },
    { 1, "scene" },
    { 2, "smart_light" },
    { 3, "curtain" },
    { 4, "thermostat" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_iwyqtclw_show_screen_dp37[] = {
    { 0, "motion" },
    { 1, "on_press" },
    { 2, "on" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_iwyqtclw_power_on_behavior_dp38[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_iwyqtclw_power_on_behavior_l1_dp39[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_iwyqtclw_power_on_behavior_l2_dp40[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_iwyqtclw_power_on_behavior_l3_dp41[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_iwyqtclw_power_on_behavior_l4_dp42[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_iwyqtclw_scene_switch_dp102[] = {
    { 0, "switch_1" },
    { 1, "switch_2" },
    { 2, "switch_3" },
    { 3, "switch_4" },
    { 4, "switch_5" },
    { 5, "switch_6" },
    { 6, "switch_7" },
    { 7, "switch_8" },
    { 8, "switch_9" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_iwyqtclw[] = {
    { 1, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 2, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "mode_l1", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_iwyqtclw_mode_l1_dp18, sizeof(kEnum__TZE284_iwyqtclw_mode_l1_dp18)/sizeof(kEnum__TZE284_iwyqtclw_mode_l1_dp18[0]) },
    { 19, "mode_l2", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_iwyqtclw_mode_l2_dp19, sizeof(kEnum__TZE284_iwyqtclw_mode_l2_dp19)/sizeof(kEnum__TZE284_iwyqtclw_mode_l2_dp19[0]) },
    { 20, "mode_l3", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_iwyqtclw_mode_l3_dp20, sizeof(kEnum__TZE284_iwyqtclw_mode_l3_dp20)/sizeof(kEnum__TZE284_iwyqtclw_mode_l3_dp20[0]) },
    { 21, "mode_l4", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_iwyqtclw_mode_l4_dp21, sizeof(kEnum__TZE284_iwyqtclw_mode_l4_dp21)/sizeof(kEnum__TZE284_iwyqtclw_mode_l4_dp21[0]) },
    { 24, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 25, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 26, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 27, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 36, "show_weather", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 37, "show_screen", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_iwyqtclw_show_screen_dp37, sizeof(kEnum__TZE284_iwyqtclw_show_screen_dp37)/sizeof(kEnum__TZE284_iwyqtclw_show_screen_dp37[0]) },
    { 38, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_iwyqtclw_power_on_behavior_dp38, sizeof(kEnum__TZE284_iwyqtclw_power_on_behavior_dp38)/sizeof(kEnum__TZE284_iwyqtclw_power_on_behavior_dp38[0]) },
    { 39, "power_on_behavior_l1", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_iwyqtclw_power_on_behavior_l1_dp39, sizeof(kEnum__TZE284_iwyqtclw_power_on_behavior_l1_dp39)/sizeof(kEnum__TZE284_iwyqtclw_power_on_behavior_l1_dp39[0]) },
    { 40, "power_on_behavior_l2", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_iwyqtclw_power_on_behavior_l2_dp40, sizeof(kEnum__TZE284_iwyqtclw_power_on_behavior_l2_dp40)/sizeof(kEnum__TZE284_iwyqtclw_power_on_behavior_l2_dp40[0]) },
    { 41, "power_on_behavior_l3", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_iwyqtclw_power_on_behavior_l3_dp41, sizeof(kEnum__TZE284_iwyqtclw_power_on_behavior_l3_dp41)/sizeof(kEnum__TZE284_iwyqtclw_power_on_behavior_l3_dp41[0]) },
    { 42, "power_on_behavior_l4", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_iwyqtclw_power_on_behavior_l4_dp42, sizeof(kEnum__TZE284_iwyqtclw_power_on_behavior_l4_dp42)/sizeof(kEnum__TZE284_iwyqtclw_power_on_behavior_l4_dp42[0]) },
    { 101, "backlight", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "scene_switch", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_iwyqtclw_scene_switch_dp102, sizeof(kEnum__TZE284_iwyqtclw_scene_switch_dp102)/sizeof(kEnum__TZE284_iwyqtclw_scene_switch_dp102[0]) },
    { 119, "curtain_switch_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 120, "curtain_switch_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 121, "curtain_switch_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 122, "curtain_switch_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 123, "dimmer_switch_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 124, "dimmer_switch_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 125, "dimmer_switch_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 126, "dimmer_switch_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 127, "thermostat", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_iwyqtclw{ kEntries__TZE284_iwyqtclw, 31 };
constexpr FzConverter kFzDp__TZE284_iwyqtclw{
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
    .user_config       = &kMap__TZE284_iwyqtclw,
};
const FzConverter* const kFz__TZE284_iwyqtclw[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_iwyqtclw,
};
constexpr TzConverter kTzDp__TZE284_iwyqtclw{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_iwyqtclw,
};
const TzConverter* const kTz__TZE284_iwyqtclw[] = { &kTzDp__TZE284_iwyqtclw };
constexpr const char* kM__TZE284_iwyqtclw[] = { "TS0601" };
constexpr const char* kN__TZE284_iwyqtclw[] = { "_TZE284_iwyqtclw" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_iwyqtclw{
    .zigbee_models=kM__TZE284_iwyqtclw,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_iwyqtclw,.manufacturer_names_count=1,
    .model="TS0601__TZE284_iwyqtclw",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_iwyqtclw,
    .from_zigbee_count=sizeof(kFz__TZE284_iwyqtclw)/sizeof(kFz__TZE284_iwyqtclw[0]),
    .to_zigbee=kTz__TZE284_iwyqtclw,
    .to_zigbee_count=sizeof(kTz__TZE284_iwyqtclw)/sizeof(kTz__TZE284_iwyqtclw[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
