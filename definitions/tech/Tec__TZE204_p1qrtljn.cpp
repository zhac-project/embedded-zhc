// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tech VNTH-T2_v2 (TS0601 / _TZE204_p1qrtljn) — DP-mapped thermostatic
// radiator valve (v2 adds DP 39 scale_protection). Hand-promoted from
// auto-generated DP extract; model string renamed from
// "TS0601__TZE204_p1qrtljn" to "VNTH-T2_v2" to match z2m source.
// Graduated out of generated/ to add the dropped system_mode channel: z2m
// fans DP 101 to BOTH `state` (on/off) AND `system_mode` (lookup
// {heat: true, off: false}). The auto extract kept only `state`; the second
// row uses the new kTuyaDpFlagBoolEnum to emit "heat"/"off".
// z2m-source: tech.ts #VNTH-T2_v2.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tech {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_p1qrtljn_running_state_dp3[] = {
    { 1, "heat" },
    { 0, "idle" },
};

// z2m DP 101 lookup({heat: true, off: false}) over the boolean state DP.
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_p1qrtljn_system_mode_dp101[] = {
    { 1, "heat" },
    { 0, "off" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_p1qrtljn_preset_dp2[] = {
    { 0, "manual" },
    { 1, "schedule" },
    { 2, "eco" },
    { 3, "comfort" },
    { 4, "antifrost" },
    { 5, "holiday" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_p1qrtljn[] = {
    { 101, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 101, "system_mode", ::zhc::TuyaDpType::Bool, 1, kEnum__TZE204_p1qrtljn_system_mode_dp101, sizeof(kEnum__TZE204_p1qrtljn_system_mode_dp101)/sizeof(kEnum__TZE204_p1qrtljn_system_mode_dp101[0]), ::zhc::tuya::kTuyaDpFlagBoolEnum },
    { 7, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_p1qrtljn_running_state_dp3, sizeof(kEnum__TZE204_p1qrtljn_running_state_dp3)/sizeof(kEnum__TZE204_p1qrtljn_running_state_dp3[0]) },
    { 5, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 47, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 6, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 102, "temperature_sensitivity", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 104, "comfort_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 103, "eco_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 21, "holiday_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 105, "min_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 36, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 39, "scale_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 14, "window_detection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 15, "window_open", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 35, "fault_alarm", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_p1qrtljn_preset_dp2, sizeof(kEnum__TZE204_p1qrtljn_preset_dp2)/sizeof(kEnum__TZE204_p1qrtljn_preset_dp2[0]) },
    { 28, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 29, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 30, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 31, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 32, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 33, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 34, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_p1qrtljn{
    kEntries__TZE204_p1qrtljn,
    sizeof(kEntries__TZE204_p1qrtljn) / sizeof(kEntries__TZE204_p1qrtljn[0]) };
constexpr FzConverter kFzDp__TZE204_p1qrtljn{
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
    .user_config       = &kMap__TZE204_p1qrtljn,
};
const FzConverter* const kFz__TZE204_p1qrtljn[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_p1qrtljn,
};
constexpr TzConverter kTzDp__TZE204_p1qrtljn{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_p1qrtljn,
};
const TzConverter* const kTz__TZE204_p1qrtljn[] = { &kTzDp__TZE204_p1qrtljn };
constexpr const char* kM__TZE204_p1qrtljn[] = { "TS0601" };
constexpr const char* kN__TZE204_p1qrtljn[] = { "_TZE204_p1qrtljn" };
}  // namespace

// --- auto-generated by emit_expose_bindings.py ---
constexpr const char* kSystemModeOpts__TZE204_p1qrtljn[] = { "heat", "off" };
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kSystemModeOpts__TZE204_p1qrtljn, 2},
    {"action", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"temperature_sensitivity", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"comfort_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"eco_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"holiday_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"min_temperature_limit", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"frost_protection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scale_protection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"window_detection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"window_open", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"fault_alarm", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"preset", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_monday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_tuesday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_wednesday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_thursday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_friday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_saturday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_sunday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDefTec__TZE204_p1qrtljn{
    .zigbee_models=kM__TZE204_p1qrtljn,.zigbee_models_count=sizeof(kM__TZE204_p1qrtljn)/sizeof(kM__TZE204_p1qrtljn[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_p1qrtljn,.manufacturer_names_count=1,
    .model="VNTH-T2_v2",.vendor="Tech",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_p1qrtljn,
    .from_zigbee_count=sizeof(kFz__TZE204_p1qrtljn)/sizeof(kFz__TZE204_p1qrtljn[0]),
    .to_zigbee=kTz__TZE204_p1qrtljn,
    .to_zigbee_count=sizeof(kTz__TZE204_p1qrtljn)/sizeof(kTz__TZE204_p1qrtljn[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::tech
