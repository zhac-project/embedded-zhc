// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_6kijc7nd).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_6kijc7nd[] = {
    { 8, "window_check", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 10, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 19, "upper_temp", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 27, "temperature_correction", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 34, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 39, "factory_reset", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 40, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 48, "week_schedule", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "switch_sensitivity", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 102, "floor_temp_protection", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 103, "floor_low_protection", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 104, "window_open_detection_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "window_open_detection_temp", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "window_open_delay_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "humidity_control", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 108, "upper_humidity_limit", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_6kijc7nd{ kEntries__TZE204_6kijc7nd, 18 };
constexpr FzConverter kFzDp__TZE204_6kijc7nd{
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
    .user_config       = &kMap__TZE204_6kijc7nd,
};
const FzConverter* const kFz__TZE204_6kijc7nd[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_6kijc7nd,
};
constexpr TzConverter kTzDp__TZE204_6kijc7nd{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_6kijc7nd,
};
const TzConverter* const kTz__TZE204_6kijc7nd[] = { &kTzDp__TZE204_6kijc7nd };
constexpr const char* kM__TZE204_6kijc7nd[] = { "TS0601" };
constexpr const char* kN__TZE204_6kijc7nd[] = { "_TZE204_6kijc7nd" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE204_6kijc7nd_2[] = {"off", "heat"};
constexpr const char* kAutoOpts__TZE204_6kijc7nd_3[] = {"idle", "heat"};
constexpr const char* kAutoOpts__TZE204_6kijc7nd_4[] = {"manual", "program"};
constexpr const char* kAutoOpts__TZE204_6kijc7nd_5[] = {"heat_mode", "cool_mode"};
constexpr const char* kAutoOpts__TZE204_6kijc7nd_6[] = {"periods_4"};
constexpr const char* kAutoOpts__TZE204_6kijc7nd_9[] = {"open", "close"};
constexpr const char* kAutoOpts__TZE204_6kijc7nd_10[] = {"Keeping Warm", "Working"};
constexpr const char* kAutoOpts__TZE204_6kijc7nd_13[] = {"in", "out"};
constexpr Expose kAutoExposes__TZE204_6kijc7nd[] = {
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_6kijc7nd_2, 2},
    {"running_state", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_6kijc7nd_3, 2},
    {"mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_6kijc7nd_4, 2},
    {"run_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_6kijc7nd_5, 2},
    {"week_program_periods", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_6kijc7nd_6, 1},
    {"factory_reset", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"window_state", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_6kijc7nd_9, 2},
    {"working_status", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_6kijc7nd_10, 2},
    {"window_check", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"frost_protection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"sensor_choose", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_6kijc7nd_13, 2},
    {"humidity_control", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"week_schedule", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"window_open_detection_time", ExposeType::Numeric, Access::StateSet, "min", nullptr, nullptr, 0, ExposeCategory::State, 2, 30, 1},
    {"window_open_detection_temp", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 2, 4, 0},
    {"window_open_delay_time", ExposeType::Numeric, Access::StateSet, "min", nullptr, nullptr, 0, ExposeCategory::State, 10, 60, 1},
    {"upper_temp", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 35, 95, 0},
    {"temperature_correction", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -9, 9, 1},
    {"switch_sensitivity", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 0, 5, 0},
    {"floor_temp_protection", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 60, 0},
    {"floor_low_protection", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 10, 30, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_6kijc7nd{
    .zigbee_models=kM__TZE204_6kijc7nd,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_6kijc7nd,.manufacturer_names_count=1,
    .model="TS0601__TZE204_6kijc7nd",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_6kijc7nd,.exposes_count=sizeof(kAutoExposes__TZE204_6kijc7nd)/sizeof(kAutoExposes__TZE204_6kijc7nd[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_6kijc7nd,
    .from_zigbee_count=sizeof(kFz__TZE204_6kijc7nd)/sizeof(kFz__TZE204_6kijc7nd[0]),
    .to_zigbee=kTz__TZE204_6kijc7nd,
    .to_zigbee_count=sizeof(kTz__TZE204_6kijc7nd)/sizeof(kTz__TZE204_6kijc7nd[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
