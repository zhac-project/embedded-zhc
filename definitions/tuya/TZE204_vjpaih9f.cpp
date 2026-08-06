// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601_thermostat_14 (_TZE204_vjpaih9f) — z2m parity v26.92.0.
// GRADUATED from definitions/tuya/generated/Gen__TZE204_vjpaih9f.cpp — z2m parity v26.92.0.
//
// Reason: local_temperature_calibration (DP47) was generated with divisor 1,
// but z2m has always applied /10 here. Until this window the triple read
// `tuya.valueConverter.localTempCalibration3`, whose body is
//     from: v => (v > 0x7fffffff ? v - 0x100000000 : v) / 10
// i.e. signed AND divided by ten. The parity tool defaulted every
// localTempCalibrationN to /1 because it could not see inside the named
// helper, so the mismatch was written off as a false flag in the previous
// window's B3 triage. v26.92.0 deleted the five localTempCalibrationN helpers
// and the triple now reads plainly as `tuya.valueConverter.divideBy10`,
// which settles it: the ez divisor was wrong and every reported calibration
// value was 10x too large (a -3.0 C offset surfaced as -30).
//
// This file is otherwise a verbatim copy of the generated definition; the only
// change is that one divisor.
// Originally auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_vjpaih9f).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_vjpaih9f_preset_dp2[] = {
    { 0, "manual" },
    { 1, "schedule" },
    { 2, "eco" },
    { 3, "comfort" },
    { 4, "frost_protection" },
    { 5, "holiday" },
    { 6, "off" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_vjpaih9f[] = {
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_vjpaih9f_preset_dp2, sizeof(kEnum__TZE204_vjpaih9f_preset_dp2)/sizeof(kEnum__TZE204_vjpaih9f_preset_dp2[0]) },
    { 4, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 5, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 6, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 14, "window_detection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 21, "holiday_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 28, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 29, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 30, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 31, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 32, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 33, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 34, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 35, "battery_low", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 36, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 47, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 103, "eco_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 104, "comfort_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 105, "frost_protection_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_vjpaih9f{ kEntries__TZE204_vjpaih9f, 20 };
constexpr FzConverter kFzDp__TZE204_vjpaih9f{
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
    .user_config       = &kMap__TZE204_vjpaih9f,
};
const FzConverter* const kFz__TZE204_vjpaih9f[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_vjpaih9f,
};
constexpr TzConverter kTzDp__TZE204_vjpaih9f{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_vjpaih9f,
};
const TzConverter* const kTz__TZE204_vjpaih9f[] = { &kTzDp__TZE204_vjpaih9f };
constexpr const char* kM__TZE204_vjpaih9f[] = { "TS0601" };
constexpr const char* kN__TZE204_vjpaih9f[] = { "_TZE204_vjpaih9f" };
}  // namespace
extern const PreparedDefinition kDef__TZE204_vjpaih9f{
    .zigbee_models=kM__TZE204_vjpaih9f,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_vjpaih9f,.manufacturer_names_count=1,
    .model="TS0601__TZE204_vjpaih9f",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_vjpaih9f,
    .from_zigbee_count=sizeof(kFz__TZE204_vjpaih9f)/sizeof(kFz__TZE204_vjpaih9f[0]),
    .to_zigbee=kTz__TZE204_vjpaih9f,
    .to_zigbee_count=sizeof(kTz__TZE204_vjpaih9f)/sizeof(kTz__TZE204_vjpaih9f[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
