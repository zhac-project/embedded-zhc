// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_x9usygq1).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_x9usygq1_mode_dp2[] = {
    { 0, "standby" },
    { 1, "comfort" },
    { 2, "comfort_1" },
    { 3, "comfort_2" },
    { 4, "eco" },
    { 5, "antifrost" },
    { 6, "program" },
    { 7, "thermostat" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_x9usygq1_system_mode_dp2[] = {
    { 0, "off" },
    { 6, "auto" },
    { 7, "heat" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_x9usygq1_preset_dp2[] = {
    { 0, "off" },
    { 1, "comfort" },
    { 2, "comfort_1" },
    { 3, "comfort_2" },
    { 4, "eco" },
    { 5, "antifrost" },
    { 6, "program" },
    { 7, "thermostat" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_x9usygq1_window_open_dp17[] = {
    { 0, "close" },
    { 1, "open" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_x9usygq1_temp_unit_convert_dp46[] = {
    { 0, "c" },
    { 1, "f" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_x9usygq1_running_mode_dp113[] = {
    { 0, "standby" },
    { 1, "comfort" },
    { 2, "comfort_1" },
    { 3, "comfort_2" },
    { 4, "eco" },
    { 5, "antifrost" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_x9usygq1[] = {
    { 2, "mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_x9usygq1_mode_dp2, sizeof(kEnum__TZE204_x9usygq1_mode_dp2)/sizeof(kEnum__TZE204_x9usygq1_mode_dp2[0]) },
    { 2, "system_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_x9usygq1_system_mode_dp2, sizeof(kEnum__TZE204_x9usygq1_system_mode_dp2)/sizeof(kEnum__TZE204_x9usygq1_system_mode_dp2[0]) },
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_x9usygq1_preset_dp2, sizeof(kEnum__TZE204_x9usygq1_preset_dp2)/sizeof(kEnum__TZE204_x9usygq1_preset_dp2[0]) },
    { 16, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 17, "window_open", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_x9usygq1_window_open_dp17, sizeof(kEnum__TZE204_x9usygq1_window_open_dp17)/sizeof(kEnum__TZE204_x9usygq1_window_open_dp17[0]) },
    { 18, "lower_temp", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 19, "temp_correction", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 19, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 20, "fault", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 29, "window_detection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 34, "upper_temp", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 39, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 46, "temp_unit_convert", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_x9usygq1_temp_unit_convert_dp46, sizeof(kEnum__TZE204_x9usygq1_temp_unit_convert_dp46)/sizeof(kEnum__TZE204_x9usygq1_temp_unit_convert_dp46[0]) },
    { 50, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 65, "week_program_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 66, "week_program_2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 67, "week_program_3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 68, "week_program_4", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 69, "week_program_5", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 70, "week_program_6", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 71, "week_program_7", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "vacation_duration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "boost_duration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "electricity_statistics", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "elec_statistics_day", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "elec_statistics_month", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "elec_statistics_year", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "average_power", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 108, "air_pressure_index", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "support_features", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 110, "window_keep_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "app_features", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 112, "switch_diff", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 113, "running_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_x9usygq1_running_mode_dp113, sizeof(kEnum__TZE204_x9usygq1_running_mode_dp113)/sizeof(kEnum__TZE204_x9usygq1_running_mode_dp113[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_x9usygq1{ kEntries__TZE204_x9usygq1, 34 };
constexpr FzConverter kFzDp__TZE204_x9usygq1{
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
    .user_config       = &kMap__TZE204_x9usygq1,
};
const FzConverter* const kFz__TZE204_x9usygq1[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_x9usygq1,
};
constexpr TzConverter kTzDp__TZE204_x9usygq1{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_x9usygq1,
};
const TzConverter* const kTz__TZE204_x9usygq1[] = { &kTzDp__TZE204_x9usygq1 };
constexpr const char* kM__TZE204_x9usygq1[] = { "TS0601" };
constexpr const char* kN__TZE204_x9usygq1[] = { "_TZE204_x9usygq1" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE204_x9usygq1_2[] = {"off", "comfort", "comfort_1", "comfort_2", "eco", "antifrost", "program", "thermostat"};
constexpr const char* kAutoOpts__TZE204_x9usygq1_3[] = {"off", "heat", "auto"};
constexpr const char* kAutoOpts__TZE204_x9usygq1_5[] = {"standby", "comfort", "comfort_1", "comfort_2", "eco", "antifrost", "program", "thermostat"};
constexpr const char* kAutoOpts__TZE204_x9usygq1_10[] = {"c", "f"};
constexpr const char* kAutoOpts__TZE204_x9usygq1_19[] = {"e1", "e2"};
constexpr const char* kAutoOpts__TZE204_x9usygq1_20[] = {"standby", "comfort", "comfort_1", "comfort_2", "eco", "antifrost"};
constexpr Expose kAutoExposes__TZE204_x9usygq1[] = {
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 30, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_x9usygq1_2, 8},
    {"system_mode", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_x9usygq1_3, 3},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -10, 10, 0},
    {"mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_x9usygq1_5, 8},
    {"window_open", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Diagnostic},
    {"fault", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"window_detection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"temp_unit_convert", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_x9usygq1_10, 2},
    {"lower_temp", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 30, 0},
    {"upper_temp", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 30, 0},
    {"electricity_statistics", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"boost_duration", ExposeType::Numeric, Access::StateSet, "min", nullptr, nullptr, 0, ExposeCategory::State, 0, 120, 0},
    {"elec_statistics_day", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"elec_statistics_month", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"elec_statistics_year", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"window_keep_time", ExposeType::Numeric, Access::StateSet, "min", nullptr, nullptr, 0, ExposeCategory::State, 0, 120, 0},
    {"fault_alarm", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_x9usygq1_19, 2},
    {"running_mode", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_x9usygq1_20, 6},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_x9usygq1{
    .zigbee_models=kM__TZE204_x9usygq1,.zigbee_models_count=sizeof(kM__TZE204_x9usygq1)/sizeof(kM__TZE204_x9usygq1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_x9usygq1,.manufacturer_names_count=1,
    .model="TS0601__TZE204_x9usygq1",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_x9usygq1,.exposes_count=sizeof(kAutoExposes__TZE204_x9usygq1)/sizeof(kAutoExposes__TZE204_x9usygq1[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_x9usygq1,
    .from_zigbee_count=sizeof(kFz__TZE204_x9usygq1)/sizeof(kFz__TZE204_x9usygq1[0]),
    .to_zigbee=kTz__TZE204_x9usygq1,
    .to_zigbee_count=sizeof(kTz__TZE204_x9usygq1)/sizeof(kTz__TZE204_x9usygq1[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
