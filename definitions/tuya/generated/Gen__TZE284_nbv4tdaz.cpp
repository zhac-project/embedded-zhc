// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_nbv4tdaz).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_nbv4tdaz_preset_dp2[] = {
    { 0, "auto" },
    { 1, "manual" },
    { 2, "holiday" },
    { 3, "eco" },
    { 4, "comfort" },
    { 5, "standby" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_nbv4tdaz_running_state_dp3[] = {
    { 0, "heat" },
    { 1, "idle" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_nbv4tdaz_window_open_dp15[] = {
    { 0, "open" },
    { 1, "closed" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_nbv4tdaz_valve_state_dp49[] = {
    { 0, "open" },
    { 1, "close" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_nbv4tdaz[] = {
    { 1, "preheat", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_nbv4tdaz_preset_dp2, sizeof(kEnum__TZE284_nbv4tdaz_preset_dp2)/sizeof(kEnum__TZE284_nbv4tdaz_preset_dp2[0]) },
    { 3, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_nbv4tdaz_running_state_dp3, sizeof(kEnum__TZE284_nbv4tdaz_running_state_dp3)/sizeof(kEnum__TZE284_nbv4tdaz_running_state_dp3[0]) },
    { 6, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "window_detection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 15, "window_open", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_nbv4tdaz_window_open_dp15, sizeof(kEnum__TZE284_nbv4tdaz_window_open_dp15)/sizeof(kEnum__TZE284_nbv4tdaz_window_open_dp15[0]) },
    { 16, "window_temp", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 18, "display_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 35, "fault_code", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 47, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 49, "valve_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_nbv4tdaz_valve_state_dp49, sizeof(kEnum__TZE284_nbv4tdaz_valve_state_dp49)/sizeof(kEnum__TZE284_nbv4tdaz_valve_state_dp49[0]) },
    { 101, "boost_heating", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 103, "eco_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 104, "comfort_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 107, "frost_protection_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 110, "battery_status", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 113, "summer_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 115, "override_active", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 28, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 29, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 30, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 31, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 32, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 33, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 34, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_nbv4tdaz{ kEntries__TZE284_nbv4tdaz, 25 };
constexpr FzConverter kFzDp__TZE284_nbv4tdaz{
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
    .user_config       = &kMap__TZE284_nbv4tdaz,
};
const FzConverter* const kFz__TZE284_nbv4tdaz[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_nbv4tdaz,
};
constexpr TzConverter kTzDp__TZE284_nbv4tdaz{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_nbv4tdaz,
};
const TzConverter* const kTz__TZE284_nbv4tdaz[] = { &kTzDp__TZE284_nbv4tdaz };
constexpr const char* kM__TZE284_nbv4tdaz[] = { "TS0601" };
constexpr const char* kN__TZE284_nbv4tdaz[] = { "_TZE284_nbv4tdaz" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_nbv4tdaz{
    .zigbee_models=kM__TZE284_nbv4tdaz,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_nbv4tdaz,.manufacturer_names_count=1,
    .model="TS0601__TZE284_nbv4tdaz",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_nbv4tdaz,
    .from_zigbee_count=sizeof(kFz__TZE284_nbv4tdaz)/sizeof(kFz__TZE284_nbv4tdaz[0]),
    .to_zigbee=kTz__TZE284_nbv4tdaz,
    .to_zigbee_count=sizeof(kTz__TZE284_nbv4tdaz)/sizeof(kTz__TZE284_nbv4tdaz[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
