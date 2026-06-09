// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Vendor-parity rewrite: Avatto ME168_AVATTO — Thermostatic radiator valve.
// z2m-source: avatto.ts #ME168_AVATTO (fingerprint TS0601 / _TZE200_ybsqljjg, _TZE200_cxakecfo, _TZE200_4aijvczq, _TZE200_r5ksy7qo).
// Tier 2: DP47 local_temperature_calibration divisor fixed 100 -> 1. z2m wires
// `localTempCalibration2`, whose decode is identity (from: (v) => v) — the value
// is already in whole degrees (withLocalTemperatureCalibration(-30,30,1)). The
// generated /100 divisor turned a +3 °C offset into 0.03 °C (100x under-read).
// (TRV26/TRV60 use `localTempCalibration1` = v/10, correctly divisor=10.)
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::avatto {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_4aijvczq_running_state_dp3[] = {
    { 0, "heat" },
    { 1, "idle" },
};

// DP2 multiplex — z2m's thermostatSystemModeAndPresetMap publishes both
// system_mode and preset from one wire value, and accepts writes on either
// key with its own toMap. After Phase A dispatcher fix the rows fire all
// matches, so we register two Enum entries — each with the z2m toMap shape.
// fz read coverage: wire 0/1/2 decode system_mode (auto/heat/off); wire
// 1/3/4/5 decode preset (none/eco/comfort/boost). The third axis z2m calls
// `deviceMode` is the cross-product of these two — derive it downstream if
// the SPA needs it.
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_4aijvczq_system_mode_dp2[] = {
    { 0, "auto" },
    { 1, "heat" },
    { 2, "off" },
};
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_4aijvczq_preset_dp2[] = {
    { 1, "none" },
    { 3, "eco" },
    { 4, "comfort" },
    { 5, "boost" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_4aijvczq[] = {
    { 2, "system_mode", ::zhc::TuyaDpType::Enum, 1,
        kEnum__TZE200_4aijvczq_system_mode_dp2,
        sizeof(kEnum__TZE200_4aijvczq_system_mode_dp2)/sizeof(kEnum__TZE200_4aijvczq_system_mode_dp2[0]) },
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1,
        kEnum__TZE200_4aijvczq_preset_dp2,
        sizeof(kEnum__TZE200_4aijvczq_preset_dp2)/sizeof(kEnum__TZE200_4aijvczq_preset_dp2[0]) },
    { 3, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_4aijvczq_running_state_dp3, sizeof(kEnum__TZE200_4aijvczq_running_state_dp3)/sizeof(kEnum__TZE200_4aijvczq_running_state_dp3[0]) },
    { 4, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 5, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 6, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 14, "window_detection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 15, "window_open", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 28, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 29, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 30, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 31, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 32, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 33, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 34, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    // DP35 — z2m's `errorOrBatteryLow` decoder splits one wire value into
    // either `battery_low` (wire 0 / 1) or `error` (wire >= 2). ZHC has no
    // bitmap-extract / multi-emit primitive today, so emit the raw value
    // as `error_status` (Numeric). Downstream derives battery_low from
    // error_status == 1 if needed. Documented in [[feedback-generator-scope]].
    { 35, "error_status", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 36, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 39, "scale_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 47, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "comfort_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 103, "boost_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "boost_timeset_countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "eco_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_4aijvczq{ kEntries__TZE200_4aijvczq, 24 };

constexpr const char* kSystemModeValues_ME168[] = { "auto", "heat", "off" };
constexpr const char* kPresetValues_ME168[] = { "none", "eco", "comfort", "boost" };
constexpr FzConverter kFzDp__TZE200_4aijvczq{
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
    .user_config       = &kMap__TZE200_4aijvczq,
};
const FzConverter* const kFz__TZE200_4aijvczq[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_4aijvczq,
};
constexpr TzConverter kTzDp__TZE200_4aijvczq{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_4aijvczq,
};
const TzConverter* const kTz__TZE200_4aijvczq[] = { &kTzDp__TZE200_4aijvczq };
constexpr const char* kM__TZE200_4aijvczq[] = { "TS0601" };
constexpr const char* kN__TZE200_4aijvczq[] = { "_TZE200_ybsqljjg", "_TZE200_cxakecfo", "_TZE200_4aijvczq", "_TZE200_r5ksy7qo" };
}  // namespace

// --- auto-generated by emit_expose_bindings.py ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr,
        "Thermostat operating mode (z2m thermostatSystemModeAndPresetMap)",
        kSystemModeValues_ME168, sizeof(kSystemModeValues_ME168)/sizeof(kSystemModeValues_ME168[0])},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr,
        "Thermostat preset",
        kPresetValues_ME168, sizeof(kPresetValues_ME168)/sizeof(kPresetValues_ME168[0])},
    {"running_state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"window_detection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"window_open", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_monday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_tuesday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_wednesday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_thursday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_friday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_saturday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_sunday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"error_status", ExposeType::Numeric, Access::State, nullptr,
        "Combined error/battery_low byte; 0=ok, 1=battery_low, >=2=error code",
        nullptr, 0},
    {"frost_protection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scale_protection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"comfort_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"boost_time", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"boost_timeset_countdown", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"eco_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDefAva__TZE200_4aijvczq{
    .zigbee_models=kM__TZE200_4aijvczq,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_4aijvczq,.manufacturer_names_count=4,
    .model="ME168_AVATTO",.vendor="Avatto",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_4aijvczq,
    .from_zigbee_count=sizeof(kFz__TZE200_4aijvczq)/sizeof(kFz__TZE200_4aijvczq[0]),
    .to_zigbee=kTz__TZE200_4aijvczq,
    .to_zigbee_count=sizeof(kTz__TZE200_4aijvczq)/sizeof(kTz__TZE200_4aijvczq[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::avatto
