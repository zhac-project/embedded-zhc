// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0225 / _TZ321C_fkzihax8).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZ321C_fkzihax8_debug_mode_dp108[] = {
    { 0, "OFF" },
    { 1, "ON" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZ321C_fkzihax8_scene_preset_dp111[] = {
    { 0, "Custom" },
    { 1, "Toilet" },
    { 2, "Kitchen" },
    { 3, "Hallway" },
    { 4, "Bedroom" },
    { 5, "Livingroom" },
    { 6, "Meetingroom" },
    { 7, "Default" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZ321C_fkzihax8_sensor_dp112[] = {
    { 0, "on" },
    { 1, "occupied" },
    { 2, "unoccupied" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZ321C_fkzihax8_status_indication_dp114[] = {
    { 0, "OFF" },
    { 1, "ON" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZ321C_fkzihax8_distance_report_mode_dp120[] = {
    { 0, "Normal" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZ321C_fkzihax8[] = {
    { 101, "entry_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "entry_distance", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 103, "departure_delay", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "entry_filter_time", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 105, "block_time", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 107, "illuminance", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 108, "debug_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZ321C_fkzihax8_debug_mode_dp108, sizeof(kEnum__TZ321C_fkzihax8_debug_mode_dp108)/sizeof(kEnum__TZ321C_fkzihax8_debug_mode_dp108[0]) },
    { 109, "debug_distance", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 110, "debug_countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "scene_preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZ321C_fkzihax8_scene_preset_dp111, sizeof(kEnum__TZ321C_fkzihax8_scene_preset_dp111)/sizeof(kEnum__TZ321C_fkzihax8_scene_preset_dp111[0]) },
    { 112, "sensor", ::zhc::TuyaDpType::Enum, 1, kEnum__TZ321C_fkzihax8_sensor_dp112, sizeof(kEnum__TZ321C_fkzihax8_sensor_dp112)/sizeof(kEnum__TZ321C_fkzihax8_sensor_dp112[0]) },
    { 113, "cline", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 114, "status_indication", ::zhc::TuyaDpType::Enum, 1, kEnum__TZ321C_fkzihax8_status_indication_dp114, sizeof(kEnum__TZ321C_fkzihax8_status_indication_dp114)/sizeof(kEnum__TZ321C_fkzihax8_status_indication_dp114[0]) },
    { 115, "radar_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 116, "shield_range", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 117, "detection_range", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 118, "equipment_status", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 119, "target_distance", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 120, "distance_report_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZ321C_fkzihax8_distance_report_mode_dp120, sizeof(kEnum__TZ321C_fkzihax8_distance_report_mode_dp120)/sizeof(kEnum__TZ321C_fkzihax8_distance_report_mode_dp120[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZ321C_fkzihax8{ kEntries__TZ321C_fkzihax8, 19 };
constexpr FzConverter kFzDp__TZ321C_fkzihax8{
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
    .user_config       = &kMap__TZ321C_fkzihax8,
};
const FzConverter* const kFz__TZ321C_fkzihax8[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZ321C_fkzihax8,
};
constexpr TzConverter kTzDp__TZ321C_fkzihax8{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZ321C_fkzihax8,
};
const TzConverter* const kTz__TZ321C_fkzihax8[] = { &kTzDp__TZ321C_fkzihax8 };
constexpr const char* kM__TZ321C_fkzihax8[] = { "TS0225" };
constexpr const char* kN__TZ321C_fkzihax8[] = { "_TZ321C_fkzihax8" };
}  // namespace
extern const PreparedDefinition kDefGen__TZ321C_fkzihax8{
    .zigbee_models=kM__TZ321C_fkzihax8,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZ321C_fkzihax8,.manufacturer_names_count=1,
    .model="TS0225__TZ321C_fkzihax8",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZ321C_fkzihax8,
    .from_zigbee_count=sizeof(kFz__TZ321C_fkzihax8)/sizeof(kFz__TZ321C_fkzihax8[0]),
    .to_zigbee=kTz__TZ321C_fkzihax8,
    .to_zigbee_count=sizeof(kTz__TZ321C_fkzihax8)/sizeof(kTz__TZ321C_fkzihax8[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
