// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_aai5grix).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_gate_enable_01_dp101[] = {
    { 0, "disable" },
    { 1, "enable" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_gate_enable_02_dp102[] = {
    { 0, "disable" },
    { 1, "enable" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_gate_enable_03_dp103[] = {
    { 0, "disable" },
    { 1, "enable" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_gate_enable_04_dp104[] = {
    { 0, "disable" },
    { 1, "enable" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_gate_enable_05_dp105[] = {
    { 0, "disable" },
    { 1, "enable" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_gate_enable_06_dp106[] = {
    { 0, "disable" },
    { 1, "enable" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_gate_enable_07_dp107[] = {
    { 0, "disable" },
    { 1, "enable" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_gate_enable_08_dp108[] = {
    { 0, "disable" },
    { 1, "enable" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_gate_enable_09_dp109[] = {
    { 0, "disable" },
    { 1, "enable" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_gate_enable_10_dp110[] = {
    { 0, "disable" },
    { 1, "enable" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_gate_enable_11_dp111[] = {
    { 0, "disable" },
    { 1, "enable" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_debug_switch_dp118[] = {
    { 0, "off" },
    { 1, "on" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_led_mode_dp119[] = {
    { 0, "silence" },
    { 1, "status" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_judge_logic_dp122[] = {
    { 0, "large_move" },
    { 1, "small_move" },
    { 2, "custom_move" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_noise_collect_status_dp123[] = {
    { 0, "start" },
    { 1, "ongoing" },
    { 2, "complete" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_device_control_dp124[] = {
    { 0, "no_action" },
    { 1, "restart" },
    { 2, "reset_param" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_presence_sensitivity_dp126[] = {
    { 0, "high" },
    { 1, "medium" },
    { 2, "low" },
    { 3, "custom" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_move_sensitivity_dp127[] = {
    { 0, "high" },
    { 1, "medium" },
    { 2, "low" },
    { 3, "custom" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_scene_mode_dp128[] = {
    { 0, "custom" },
    { 1, "toilet" },
    { 2, "kitchen" },
    { 3, "corridor" },
    { 4, "bedroom" },
    { 5, "living_room" },
    { 6, "meeting_room" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_illuminance_report_dp129[] = {
    { 0, "off" },
    { 1, "on" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_move_detect_dp130[] = {
    { 0, "off" },
    { 1, "on" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_distance_report_dp131[] = {
    { 0, "off" },
    { 1, "on" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_aai5grix_speed_report_dp132[] = {
    { 0, "off" },
    { 1, "on" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_aai5grix[] = {
    { 3, "min_distance", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 4, "max_distance", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 9, "target_distance", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 101, "gate_enable_01", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_gate_enable_01_dp101, sizeof(kEnum__TZE284_aai5grix_gate_enable_01_dp101)/sizeof(kEnum__TZE284_aai5grix_gate_enable_01_dp101[0]) },
    { 102, "gate_enable_02", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_gate_enable_02_dp102, sizeof(kEnum__TZE284_aai5grix_gate_enable_02_dp102)/sizeof(kEnum__TZE284_aai5grix_gate_enable_02_dp102[0]) },
    { 103, "gate_enable_03", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_gate_enable_03_dp103, sizeof(kEnum__TZE284_aai5grix_gate_enable_03_dp103)/sizeof(kEnum__TZE284_aai5grix_gate_enable_03_dp103[0]) },
    { 104, "gate_enable_04", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_gate_enable_04_dp104, sizeof(kEnum__TZE284_aai5grix_gate_enable_04_dp104)/sizeof(kEnum__TZE284_aai5grix_gate_enable_04_dp104[0]) },
    { 105, "gate_enable_05", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_gate_enable_05_dp105, sizeof(kEnum__TZE284_aai5grix_gate_enable_05_dp105)/sizeof(kEnum__TZE284_aai5grix_gate_enable_05_dp105[0]) },
    { 106, "gate_enable_06", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_gate_enable_06_dp106, sizeof(kEnum__TZE284_aai5grix_gate_enable_06_dp106)/sizeof(kEnum__TZE284_aai5grix_gate_enable_06_dp106[0]) },
    { 107, "gate_enable_07", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_gate_enable_07_dp107, sizeof(kEnum__TZE284_aai5grix_gate_enable_07_dp107)/sizeof(kEnum__TZE284_aai5grix_gate_enable_07_dp107[0]) },
    { 108, "gate_enable_08", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_gate_enable_08_dp108, sizeof(kEnum__TZE284_aai5grix_gate_enable_08_dp108)/sizeof(kEnum__TZE284_aai5grix_gate_enable_08_dp108[0]) },
    { 109, "gate_enable_09", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_gate_enable_09_dp109, sizeof(kEnum__TZE284_aai5grix_gate_enable_09_dp109)/sizeof(kEnum__TZE284_aai5grix_gate_enable_09_dp109[0]) },
    { 110, "gate_enable_10", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_gate_enable_10_dp110, sizeof(kEnum__TZE284_aai5grix_gate_enable_10_dp110)/sizeof(kEnum__TZE284_aai5grix_gate_enable_10_dp110[0]) },
    { 111, "gate_enable_11", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_gate_enable_11_dp111, sizeof(kEnum__TZE284_aai5grix_gate_enable_11_dp111)/sizeof(kEnum__TZE284_aai5grix_gate_enable_11_dp111[0]) },
    { 112, "configuration_gate", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 113, "move_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 114, "presence_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 115, "nearest_target_gate", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 116, "target_countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 117, "target_velocity", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 118, "debug_switch", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_debug_switch_dp118, sizeof(kEnum__TZE284_aai5grix_debug_switch_dp118)/sizeof(kEnum__TZE284_aai5grix_debug_switch_dp118[0]) },
    { 119, "led_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_led_mode_dp119, sizeof(kEnum__TZE284_aai5grix_led_mode_dp119)/sizeof(kEnum__TZE284_aai5grix_led_mode_dp119[0]) },
    { 120, "delay_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 121, "block_time", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 122, "judge_logic", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_judge_logic_dp122, sizeof(kEnum__TZE284_aai5grix_judge_logic_dp122)/sizeof(kEnum__TZE284_aai5grix_judge_logic_dp122[0]) },
    { 123, "noise_collect_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_noise_collect_status_dp123, sizeof(kEnum__TZE284_aai5grix_noise_collect_status_dp123)/sizeof(kEnum__TZE284_aai5grix_noise_collect_status_dp123[0]) },
    { 124, "device_control", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_device_control_dp124, sizeof(kEnum__TZE284_aai5grix_device_control_dp124)/sizeof(kEnum__TZE284_aai5grix_device_control_dp124[0]) },
    { 125, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 126, "presence_sensitivity", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_presence_sensitivity_dp126, sizeof(kEnum__TZE284_aai5grix_presence_sensitivity_dp126)/sizeof(kEnum__TZE284_aai5grix_presence_sensitivity_dp126[0]) },
    { 127, "move_sensitivity", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_move_sensitivity_dp127, sizeof(kEnum__TZE284_aai5grix_move_sensitivity_dp127)/sizeof(kEnum__TZE284_aai5grix_move_sensitivity_dp127[0]) },
    { 128, "scene_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_scene_mode_dp128, sizeof(kEnum__TZE284_aai5grix_scene_mode_dp128)/sizeof(kEnum__TZE284_aai5grix_scene_mode_dp128[0]) },
    { 129, "illuminance_report", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_illuminance_report_dp129, sizeof(kEnum__TZE284_aai5grix_illuminance_report_dp129)/sizeof(kEnum__TZE284_aai5grix_illuminance_report_dp129[0]) },
    { 130, "move_detect", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_move_detect_dp130, sizeof(kEnum__TZE284_aai5grix_move_detect_dp130)/sizeof(kEnum__TZE284_aai5grix_move_detect_dp130[0]) },
    { 131, "distance_report", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_distance_report_dp131, sizeof(kEnum__TZE284_aai5grix_distance_report_dp131)/sizeof(kEnum__TZE284_aai5grix_distance_report_dp131[0]) },
    { 132, "speed_report", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_aai5grix_speed_report_dp132, sizeof(kEnum__TZE284_aai5grix_speed_report_dp132)/sizeof(kEnum__TZE284_aai5grix_speed_report_dp132[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_aai5grix{ kEntries__TZE284_aai5grix, 35 };
constexpr FzConverter kFzDp__TZE284_aai5grix{
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
    .user_config       = &kMap__TZE284_aai5grix,
};
const FzConverter* const kFz__TZE284_aai5grix[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_aai5grix,
};
constexpr TzConverter kTzDp__TZE284_aai5grix{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_aai5grix,
};
const TzConverter* const kTz__TZE284_aai5grix[] = { &kTzDp__TZE284_aai5grix };
constexpr const char* kM__TZE284_aai5grix[] = { "TS0601" };
constexpr const char* kN__TZE284_aai5grix[] = { "_TZE284_aai5grix" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_aai5grix{
    .zigbee_models=kM__TZE284_aai5grix,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_aai5grix,.manufacturer_names_count=1,
    .model="TS0601__TZE284_aai5grix",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_aai5grix,
    .from_zigbee_count=sizeof(kFz__TZE284_aai5grix)/sizeof(kFz__TZE284_aai5grix[0]),
    .to_zigbee=kTz__TZE284_aai5grix,
    .to_zigbee_count=sizeof(kTz__TZE284_aai5grix)/sizeof(kTz__TZE284_aai5grix[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
