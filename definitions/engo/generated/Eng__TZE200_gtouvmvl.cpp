// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/engo.ts (fingerprint TS0601 / _TZE200_gtouvmvl).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::engo {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_gtouvmvl_system_mode_dp2[] = {
    { 0, "heat" },
    { 1, "cool" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_gtouvmvl_running_state_dp3[] = {
    { 1, "heat" },
    { 2, "idle" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_gtouvmvl_sensor_choose_dp43[] = {
    { 0, "internal" },
    { 1, "floor_temp" },
    { 2, "external" },
    { 3, "external_on_off" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_gtouvmvl_preset_dp58[] = {
    { 0, "manual" },
    { 1, "schedule" },
    { 2, "holiday" },
    { 5, "frost" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_gtouvmvl_control_algorithm_dp101[] = {
    { 0, "TPI_UFH" },
    { 1, "TPI_RAD" },
    { 2, "TPI_ELE" },
    { 3, "HIS_02" },
    { 4, "HIS_04" },
    { 5, "HIS_06" },
    { 6, "HIS_08" },
    { 7, "HIS_10" },
    { 8, "HIS_20" },
    { 9, "HIS_30" },
    { 10, "HIS_40" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_gtouvmvl_valve_protection_dp108[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "anti_stop" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_gtouvmvl_temp_resolution_dp117[] = {
    { 0, "one" },
    { 1, "five" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_gtouvmvl_comfort_warm_floor_dp118[] = {
    { 0, "OFF" },
    { 1, "LEVEL1" },
    { 2, "LEVEL2" },
    { 3, "LEVEL3" },
    { 4, "LEVEL4" },
    { 5, "LEVEL5" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_gtouvmvl_sensor_error_dp120[] = {
    { 0, "normal" },
    { 1, "E1" },
    { 2, "E2" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_gtouvmvl[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "system_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_gtouvmvl_system_mode_dp2, sizeof(kEnum__TZE200_gtouvmvl_system_mode_dp2)/sizeof(kEnum__TZE200_gtouvmvl_system_mode_dp2[0]) },
    { 3, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_gtouvmvl_running_state_dp3, sizeof(kEnum__TZE200_gtouvmvl_running_state_dp3)/sizeof(kEnum__TZE200_gtouvmvl_running_state_dp3[0]) },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 19, "max_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 26, "min_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 27, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 32, "holiday_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 33, "holiday_days", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 34, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 35, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 40, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 43, "sensor_choose", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_gtouvmvl_sensor_choose_dp43, sizeof(kEnum__TZE200_gtouvmvl_sensor_choose_dp43)/sizeof(kEnum__TZE200_gtouvmvl_sensor_choose_dp43[0]) },
    { 44, "backlight", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 58, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_gtouvmvl_preset_dp58, sizeof(kEnum__TZE200_gtouvmvl_preset_dp58)/sizeof(kEnum__TZE200_gtouvmvl_preset_dp58[0]) },
    { 101, "control_algorithm", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_gtouvmvl_control_algorithm_dp101, sizeof(kEnum__TZE200_gtouvmvl_control_algorithm_dp101)/sizeof(kEnum__TZE200_gtouvmvl_control_algorithm_dp101[0]) },
    { 102, "max_floor_temp_heating", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 103, "min_floor_temp_heating", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 104, "max_floor_temp_cooling", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 105, "min_floor_temp_cooling", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 106, "frost_set", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 108, "valve_protection", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_gtouvmvl_valve_protection_dp108, sizeof(kEnum__TZE200_gtouvmvl_valve_protection_dp108)/sizeof(kEnum__TZE200_gtouvmvl_valve_protection_dp108[0]) },
    { 109, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 110, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 111, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 112, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 113, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 114, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 115, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 116, "floor_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 117, "temp_resolution", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_gtouvmvl_temp_resolution_dp117, sizeof(kEnum__TZE200_gtouvmvl_temp_resolution_dp117)/sizeof(kEnum__TZE200_gtouvmvl_temp_resolution_dp117[0]) },
    { 118, "comfort_warm_floor", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_gtouvmvl_comfort_warm_floor_dp118, sizeof(kEnum__TZE200_gtouvmvl_comfort_warm_floor_dp118)/sizeof(kEnum__TZE200_gtouvmvl_comfort_warm_floor_dp118[0]) },
    { 120, "sensor_error", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_gtouvmvl_sensor_error_dp120, sizeof(kEnum__TZE200_gtouvmvl_sensor_error_dp120)/sizeof(kEnum__TZE200_gtouvmvl_sensor_error_dp120[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_gtouvmvl{ kEntries__TZE200_gtouvmvl, 34 };
constexpr FzConverter kFzDp__TZE200_gtouvmvl{
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
    .user_config       = &kMap__TZE200_gtouvmvl,
};
const FzConverter* const kFz__TZE200_gtouvmvl[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_gtouvmvl,
};
constexpr TzConverter kTzDp__TZE200_gtouvmvl{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_gtouvmvl,
};
const TzConverter* const kTz__TZE200_gtouvmvl[] = { &kTzDp__TZE200_gtouvmvl };
constexpr const char* kM__TZE200_gtouvmvl[] = { "TS0601" };
constexpr const char* kN__TZE200_gtouvmvl[] = { "_TZE200_gtouvmvl" };
}  // namespace

// --- auto-generated by emit_expose_bindings.py ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"system_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"max_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"min_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"holiday_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"holiday_days", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"sensor_choose", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"backlight", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"preset", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"control_algorithm", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"max_floor_temp_heating", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"min_floor_temp_heating", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"max_floor_temp_cooling", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"min_floor_temp_cooling", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"frost_set", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"valve_protection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_monday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_tuesday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_wednesday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_thursday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_friday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_saturday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_sunday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"floor_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"temp_resolution", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"comfort_warm_floor", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"sensor_error", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDefEng__TZE200_gtouvmvl{
    .zigbee_models=kM__TZE200_gtouvmvl,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_gtouvmvl,.manufacturer_names_count=1,
    .model="EONE-BATB",.vendor="Engo",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_gtouvmvl,
    .from_zigbee_count=sizeof(kFz__TZE200_gtouvmvl)/sizeof(kFz__TZE200_gtouvmvl[0]),
    .to_zigbee=kTz__TZE200_gtouvmvl,
    .to_zigbee_count=sizeof(kTz__TZE200_gtouvmvl)/sizeof(kTz__TZE200_gtouvmvl[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::engo
