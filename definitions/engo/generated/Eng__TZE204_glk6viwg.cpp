// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/engo.ts (fingerprint TS0601 / _TZE204_glk6viwg).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::engo {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_glk6viwg_running_state_dp3[] = {
    { 2, "heat" },
    { 3, "cool" },
    { 4, "idle" },
    { 0, "idle_0" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_glk6viwg_sensor_choose_dp43[] = {
    { 0, "internal" },
    { 1, "all" },
    { 2, "external" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_glk6viwg_comfort_warm_floor_dp45[] = {
    { 0, "OFF" },
    { 1, "LEVEL1" },
    { 2, "LEVEL2" },
    { 3, "LEVEL3" },
    { 4, "LEVEL4" },
    { 5, "LEVEL5" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_glk6viwg_preset_dp58[] = {
    { 0, "manual" },
    { 1, "schedule" },
    { 3, "frost" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_glk6viwg_control_algorithm_dp101[] = {
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

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_glk6viwg_device_pair_state_dp103[] = {
    { 0, "none" },
    { 1, "commutation_center" },
    { 2, "trv" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_glk6viwg_valve_protection_dp107[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "anti_stop" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_glk6viwg_relay_mode_dp108[] = {
    { 0, "NO" },
    { 1, "NC" },
    { 2, "OFF" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_glk6viwg_sensor_error_dp120[] = {
    { 0, "normal" },
    { 1, "E1" },
    { 2, "E2" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_glk6viwg[] = {
    { 3, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_glk6viwg_running_state_dp3, sizeof(kEnum__TZE204_glk6viwg_running_state_dp3)/sizeof(kEnum__TZE204_glk6viwg_running_state_dp3[0]) },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 19, "max_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 26, "min_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 40, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 43, "sensor_choose", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_glk6viwg_sensor_choose_dp43, sizeof(kEnum__TZE204_glk6viwg_sensor_choose_dp43)/sizeof(kEnum__TZE204_glk6viwg_sensor_choose_dp43[0]) },
    { 44, "backlight", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 45, "comfort_warm_floor", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_glk6viwg_comfort_warm_floor_dp45, sizeof(kEnum__TZE204_glk6viwg_comfort_warm_floor_dp45)/sizeof(kEnum__TZE204_glk6viwg_comfort_warm_floor_dp45[0]) },
    { 58, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_glk6viwg_preset_dp58, sizeof(kEnum__TZE204_glk6viwg_preset_dp58)/sizeof(kEnum__TZE204_glk6viwg_preset_dp58[0]) },
    { 101, "control_algorithm", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_glk6viwg_control_algorithm_dp101, sizeof(kEnum__TZE204_glk6viwg_control_algorithm_dp101)/sizeof(kEnum__TZE204_glk6viwg_control_algorithm_dp101[0]) },
    { 102, "delta_t_rcwc_alg", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 103, "device_pair_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_glk6viwg_device_pair_state_dp103, sizeof(kEnum__TZE204_glk6viwg_device_pair_state_dp103)/sizeof(kEnum__TZE204_glk6viwg_device_pair_state_dp103[0]) },
    { 105, "temperature_set_enabled_while_blocked", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 106, "frost_set", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 107, "valve_protection", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_glk6viwg_valve_protection_dp107, sizeof(kEnum__TZE204_glk6viwg_valve_protection_dp107)/sizeof(kEnum__TZE204_glk6viwg_valve_protection_dp107[0]) },
    { 108, "relay_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_glk6viwg_relay_mode_dp108, sizeof(kEnum__TZE204_glk6viwg_relay_mode_dp108)/sizeof(kEnum__TZE204_glk6viwg_relay_mode_dp108[0]) },
    { 109, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 110, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 111, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 112, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 113, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 114, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 115, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 120, "sensor_error", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_glk6viwg_sensor_error_dp120, sizeof(kEnum__TZE204_glk6viwg_sensor_error_dp120)/sizeof(kEnum__TZE204_glk6viwg_sensor_error_dp120[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_glk6viwg{ kEntries__TZE204_glk6viwg, 25 };
constexpr FzConverter kFzDp__TZE204_glk6viwg{
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
    .user_config       = &kMap__TZE204_glk6viwg,
};
const FzConverter* const kFz__TZE204_glk6viwg[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_glk6viwg,
};
constexpr TzConverter kTzDp__TZE204_glk6viwg{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_glk6viwg,
};
const TzConverter* const kTz__TZE204_glk6viwg[] = { &kTzDp__TZE204_glk6viwg };
constexpr const char* kM__TZE204_glk6viwg[] = { "TS0601" };
constexpr const char* kN__TZE204_glk6viwg[] = { "_TZE204_glk6viwg" };
}  // namespace

// --- auto-generated by emit_expose_bindings.py ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"running_state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"max_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"min_temperature", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"sensor_choose", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"backlight", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"comfort_warm_floor", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"preset", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"control_algorithm", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"delta_t_rcwc_alg", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"device_pair_state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"temperature_set_enabled_while_blocked", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"frost_set", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"valve_protection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"relay_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_monday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_tuesday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_wednesday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_thursday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_friday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_saturday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"schedule_sunday", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"sensor_error", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDefEng__TZE204_glk6viwg{
    .zigbee_models=kM__TZE204_glk6viwg,.zigbee_models_count=sizeof(kM__TZE204_glk6viwg)/sizeof(kM__TZE204_glk6viwg[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_glk6viwg,.manufacturer_names_count=1,
    .model="E40",.vendor="Engo",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_glk6viwg,
    .from_zigbee_count=sizeof(kFz__TZE204_glk6viwg)/sizeof(kFz__TZE204_glk6viwg[0]),
    .to_zigbee=kTz__TZE204_glk6viwg,
    .to_zigbee_count=sizeof(kTz__TZE204_glk6viwg)/sizeof(kTz__TZE204_glk6viwg[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::engo
