// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_0hg58wyk).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_0hg58wyk_preset_dp2[] = {
    { 0, "manual" },
    { 1, "holiday" },
    { 2, "program" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_0hg58wyk[] = {
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_0hg58wyk_preset_dp2, sizeof(kEnum__TZE200_0hg58wyk_preset_dp2)/sizeof(kEnum__TZE200_0hg58wyk_preset_dp2[0]) },
    { 8, "open_window", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 10, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 27, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 35, "battery_low", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 40, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 45, "error_status", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 102, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 103, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 104, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 105, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 106, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 107, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_0hg58wyk{ kEntries__TZE200_0hg58wyk, 16 };
constexpr FzConverter kFzDp__TZE200_0hg58wyk{
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
    .user_config       = &kMap__TZE200_0hg58wyk,
};
const FzConverter* const kFz__TZE200_0hg58wyk[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_0hg58wyk,
};
constexpr TzConverter kTzDp__TZE200_0hg58wyk{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_0hg58wyk,
};
const TzConverter* const kTz__TZE200_0hg58wyk[] = { &kTzDp__TZE200_0hg58wyk };
constexpr const char* kM__TZE200_0hg58wyk[] = { "TS0601" };
constexpr const char* kN__TZE200_0hg58wyk[] = { "_TZE200_0hg58wyk" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_0hg58wyk_5[] = {"off", "heat"};
constexpr const char* kAutoOpts__TZE200_0hg58wyk_6[] = {"manual", "holiday", "program"};
constexpr Expose kAutoExposes__TZE200_0hg58wyk[] = {
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Diagnostic},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"open_window", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"frost_protection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"error_status", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_0hg58wyk_5, 2},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_0hg58wyk_6, 3},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -5, 5, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 30, 0},
    {"schedule_monday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_tuesday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_wednesday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_thursday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_friday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_saturday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_sunday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_0hg58wyk[] = {
    {"S366","Cloud Even"},
};
extern const PreparedDefinition kDefGen__TZE200_0hg58wyk{
    .zigbee_models=kM__TZE200_0hg58wyk,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_0hg58wyk,.manufacturer_names_count=1,
    .model="TS0601__TZE200_0hg58wyk",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_0hg58wyk,.exposes_count=sizeof(kAutoExposes__TZE200_0hg58wyk)/sizeof(kAutoExposes__TZE200_0hg58wyk[0]),
    .white_labels=kWhiteLabels_Gen_TZE200_0hg58wyk, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_0hg58wyk)/sizeof(kWhiteLabels_Gen_TZE200_0hg58wyk[0]),
    .from_zigbee=kFz__TZE200_0hg58wyk,
    .from_zigbee_count=sizeof(kFz__TZE200_0hg58wyk)/sizeof(kFz__TZE200_0hg58wyk[0]),
    .to_zigbee=kTz__TZE200_0hg58wyk,
    .to_zigbee_count=sizeof(kTz__TZE200_0hg58wyk)/sizeof(kTz__TZE200_0hg58wyk[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
