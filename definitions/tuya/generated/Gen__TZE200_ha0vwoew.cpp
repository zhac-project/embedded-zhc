// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_ha0vwoew).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_ha0vwoew_boost_dp4[] = {
    { 1, "OFF" },
    { 2, "ON" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_ha0vwoew_sensor_mode_dp43[] = {
    { 0, "room_temperature" },
    { 1, "floor_temperature" },
    { 2, "room_with_floor_limit" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_ha0vwoew[] = {
    { 2, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 4, "boost", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_ha0vwoew_boost_dp4, sizeof(kEnum__TZE200_ha0vwoew_boost_dp4)/sizeof(kEnum__TZE200_ha0vwoew_boost_dp4[0]) },
    { 45, "open_window_drop_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 38, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 39, "max_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 42, "holiday_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 43, "sensor_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_ha0vwoew_sensor_mode_dp43, sizeof(kEnum__TZE200_ha0vwoew_sensor_mode_dp43)/sizeof(kEnum__TZE200_ha0vwoew_sensor_mode_dp43[0]) },
    { 53, "min_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_ha0vwoew{ kEntries__TZE200_ha0vwoew, 8 };
constexpr FzConverter kFzDp__TZE200_ha0vwoew{
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
    .user_config       = &kMap__TZE200_ha0vwoew,
};
const FzConverter* const kFz__TZE200_ha0vwoew[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_ha0vwoew,
};
constexpr TzConverter kTzDp__TZE200_ha0vwoew{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_ha0vwoew,
};
const TzConverter* const kTz__TZE200_ha0vwoew[] = { &kTzDp__TZE200_ha0vwoew };
constexpr const char* kM__TZE200_ha0vwoew[] = { "TS0601" };
constexpr const char* kN__TZE200_ha0vwoew[] = { "_TZE200_ha0vwoew" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_ha0vwoew_0[] = {"off", "auto"};
constexpr const char* kAutoOpts__TZE200_ha0vwoew_3[] = {"room_temperature", "floor_temperature", "room_with_floor_limit"};
constexpr const char* kAutoOpts__TZE200_ha0vwoew_7[] = {"ON", "OFF"};
constexpr Expose kAutoExposes__TZE200_ha0vwoew[] = {
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_ha0vwoew_0, 2},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"sensor_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_ha0vwoew_3, 3},
    {"adaptive_start", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"max_temperature_limit", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"min_temperature_limit", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 1, 5, 0},
    {"boost", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_ha0vwoew_7, 2},
    {"display_brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 1},
    {"holiday_start_stop", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 99, 0},
    {"holiday_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"frost_protection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch_delay", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 10, 90, 10},
    {"power_rating", ExposeType::Numeric, Access::StateSet, "W", nullptr, nullptr, 0, ExposeCategory::State, 0, 4500, 100},
    {"open_window_active", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"open_window_sensing_time", ExposeType::Numeric, Access::StateSet, "minutes", nullptr, nullptr, 0, ExposeCategory::State, 1, 30, 1},
    {"open_window_drop_limit", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0, ExposeCategory::State, 2, 4, 1},
    {"open_window_off_time", ExposeType::Numeric, Access::StateSet, "minutes", nullptr, nullptr, 0, ExposeCategory::State, 10, 60, 5},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_ha0vwoew{
    .zigbee_models=kM__TZE200_ha0vwoew,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_ha0vwoew,.manufacturer_names_count=1,
    .model="TS0601__TZE200_ha0vwoew",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_ha0vwoew,.exposes_count=sizeof(kAutoExposes__TZE200_ha0vwoew)/sizeof(kAutoExposes__TZE200_ha0vwoew[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_ha0vwoew,
    .from_zigbee_count=sizeof(kFz__TZE200_ha0vwoew)/sizeof(kFz__TZE200_ha0vwoew[0]),
    .to_zigbee=kTz__TZE200_ha0vwoew,
    .to_zigbee_count=sizeof(kTz__TZE200_ha0vwoew)/sizeof(kTz__TZE200_ha0vwoew[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
