// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_eekpf0ft).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_eekpf0ft_preset_dp2[] = {
    { 0, "manual" },
    { 1, "schedule" },
    { 2, "eco" },
    { 3, "comfort" },
    { 4, "frost_protection" },
    { 5, "holiday" },
    { 6, "off" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_eekpf0ft_running_state_dp3[] = {
    { 0, "idle" },
    { 1, "heat" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_eekpf0ft_window_open_dp15[] = {
    { 0, "CLOSE" },
    { 1, "OPEN" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_eekpf0ft[] = {
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_eekpf0ft_preset_dp2, sizeof(kEnum__TZE284_eekpf0ft_preset_dp2)/sizeof(kEnum__TZE284_eekpf0ft_preset_dp2[0]) },
    { 3, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_eekpf0ft_running_state_dp3, sizeof(kEnum__TZE284_eekpf0ft_running_state_dp3)/sizeof(kEnum__TZE284_eekpf0ft_running_state_dp3[0]) },
    { 4, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 5, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 6, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 14, "window_detection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 15, "window_open", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_eekpf0ft_window_open_dp15, sizeof(kEnum__TZE284_eekpf0ft_window_open_dp15)/sizeof(kEnum__TZE284_eekpf0ft_window_open_dp15[0]) },
    { 21, "holiday_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 28, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 29, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 30, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 31, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 32, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 33, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 34, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 35, "fault_alarm", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 36, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 39, "scale_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 47, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "temperature_accuracy", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 103, "eco_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 104, "comfort_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 105, "frost_protection_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_eekpf0ft{ kEntries__TZE284_eekpf0ft, 24 };
constexpr FzConverter kFzDp__TZE284_eekpf0ft{
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
    .user_config       = &kMap__TZE284_eekpf0ft,
};
const FzConverter* const kFz__TZE284_eekpf0ft[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_eekpf0ft,
};
constexpr TzConverter kTzDp__TZE284_eekpf0ft{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_eekpf0ft,
};
const TzConverter* const kTz__TZE284_eekpf0ft[] = { &kTzDp__TZE284_eekpf0ft };
constexpr const char* kM__TZE284_eekpf0ft[] = { "TS0601" };
constexpr const char* kN__TZE284_eekpf0ft[] = { "_TZE284_eekpf0ft" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_eekpf0ft{
    .zigbee_models=kM__TZE284_eekpf0ft,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_eekpf0ft,.manufacturer_names_count=1,
    .model="TS0601__TZE284_eekpf0ft",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_eekpf0ft,
    .from_zigbee_count=sizeof(kFz__TZE284_eekpf0ft)/sizeof(kFz__TZE284_eekpf0ft[0]),
    .to_zigbee=kTz__TZE284_eekpf0ft,
    .to_zigbee_count=sizeof(kTz__TZE284_eekpf0ft)/sizeof(kTz__TZE284_eekpf0ft[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
