// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Vendor-parity rewrite: Avatto TRV26 — Thermostatic radiator valve.
// z2m-source: avatto.ts #TRV26 (fingerprint TS0601 / _TZE204_xdtnpp1a, _TZE284_xdtnpp1a).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::avatto {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_xdtnpp1a_running_state_dp49[] = {
    { 1, "heat" },
    { 0, "idle" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_xdtnpp1a_system_mode_dp49[] = {
    { 1, "heat" },
    { 0, "off" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_xdtnpp1a_preset_dp2[] = {
    { 0, "manual" },
    { 1, "auto" },
    { 2, "eco" },
    { 3, "comfort" },
    { 4, "antifrost" },
    { 5, "holiday" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_xdtnpp1a[] = {
    { 49, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_xdtnpp1a_running_state_dp49, sizeof(kEnum__TZE204_xdtnpp1a_running_state_dp49)/sizeof(kEnum__TZE204_xdtnpp1a_running_state_dp49[0]) },
    { 49, "system_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_xdtnpp1a_system_mode_dp49, sizeof(kEnum__TZE204_xdtnpp1a_system_mode_dp49)/sizeof(kEnum__TZE204_xdtnpp1a_system_mode_dp49[0]) },
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_xdtnpp1a_preset_dp2, sizeof(kEnum__TZE204_xdtnpp1a_preset_dp2)/sizeof(kEnum__TZE204_xdtnpp1a_preset_dp2[0]) },
    { 4, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 5, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 6, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 9, "max_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 10, "min_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 14, "window_detection", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "open_window_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 17, "open_window_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 19, "factory_reset", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 21, "holiday_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 24, "comfort_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 25, "eco_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 17, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 18, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 19, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 20, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 21, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 22, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 23, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 35, "error_status", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 36, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 39, "scale_protection", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 47, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 101, "uptime", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 102, "descale_countdown", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_xdtnpp1a{ kEntries__TZE204_xdtnpp1a, 29 };
constexpr FzConverter kFzDp__TZE204_xdtnpp1a{
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
    .user_config       = &kMap__TZE204_xdtnpp1a,
};
const FzConverter* const kFz__TZE204_xdtnpp1a[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_xdtnpp1a,
};
constexpr TzConverter kTzDp__TZE204_xdtnpp1a{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_xdtnpp1a,
};
const TzConverter* const kTz__TZE204_xdtnpp1a[] = { &kTzDp__TZE204_xdtnpp1a };
constexpr const char* kM__TZE204_xdtnpp1a[] = { "TS0601" };
constexpr const char* kN__TZE204_xdtnpp1a[] = { "_TZE204_xdtnpp1a", "_TZE284_xdtnpp1a" };
}  // namespace

// --- auto-generated by emit_expose_bindings.py ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"running_state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"system_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"preset", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"max_temperature_limit", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"min_temperature_limit", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"window_detection", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"open_window_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"open_window_time", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"factory_reset", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"holiday_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"comfort_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"eco_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_monday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_tuesday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_wednesday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_thursday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_friday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_saturday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_sunday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"error_status", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"frost_protection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scale_protection", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"uptime", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"descale_countdown", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDefAva__TZE204_xdtnpp1a{
    .zigbee_models=kM__TZE204_xdtnpp1a,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_xdtnpp1a,.manufacturer_names_count=2,
    .model="TRV26",.vendor="Avatto",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_xdtnpp1a,
    .from_zigbee_count=sizeof(kFz__TZE204_xdtnpp1a)/sizeof(kFz__TZE204_xdtnpp1a[0]),
    .to_zigbee=kTz__TZE204_xdtnpp1a,
    .to_zigbee_count=sizeof(kTz__TZE204_xdtnpp1a)/sizeof(kTz__TZE204_xdtnpp1a[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::avatto
