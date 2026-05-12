// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/moes.ts (fingerprint TS0601 / _TZE204_x9usygq1).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::moes {
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
}  // namespace

// --- auto-generated by emit_expose_bindings.py ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"system_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"preset", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"window_open", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"lower_temp", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"temp_correction", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"fault", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"window_detection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"upper_temp", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"temp_unit_convert", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"week_program_1", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"week_program_2", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"week_program_3", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"week_program_4", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"week_program_5", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"week_program_6", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"week_program_7", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"vacation_duration", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"boost_duration", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"electricity_statistics", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"elec_statistics_day", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"elec_statistics_month", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"elec_statistics_year", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"average_power", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"air_pressure_index", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"support_features", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"window_keep_time", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"app_features", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"switch_diff", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"running_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDefMoes__TZE204_x9usygq1{
    .zigbee_models=kM__TZE204_x9usygq1,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_x9usygq1,.manufacturer_names_count=1,
    .model="TS0601__TZE204_x9usygq1",.vendor="Moes",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_x9usygq1,
    .from_zigbee_count=sizeof(kFz__TZE204_x9usygq1)/sizeof(kFz__TZE204_x9usygq1[0]),
    .to_zigbee=kTz__TZE204_x9usygq1,
    .to_zigbee_count=sizeof(kTz__TZE204_x9usygq1)/sizeof(kTz__TZE204_x9usygq1[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::moes
