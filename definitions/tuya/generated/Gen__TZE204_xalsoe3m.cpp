// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_xalsoe3m).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_xalsoe3m_system_mode_dp2[] = {
    { 0, "auto" },
    { 1, "manual" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_xalsoe3m_sensor_dp32[] = {
    { 0, "IN" },
    { 2, "AL" },
    { 1, "OU" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_xalsoe3m_valve_state_dp47[] = {
    { 0, "closed" },
    { 1, "open" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_xalsoe3m[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "system_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_xalsoe3m_system_mode_dp2, sizeof(kEnum__TZE204_xalsoe3m_system_mode_dp2)/sizeof(kEnum__TZE204_xalsoe3m_system_mode_dp2[0]) },
    { 16, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 18, "min_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 19, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 32, "sensor", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_xalsoe3m_sensor_dp32, sizeof(kEnum__TZE204_xalsoe3m_sensor_dp32)/sizeof(kEnum__TZE204_xalsoe3m_sensor_dp32[0]) },
    { 34, "max_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 39, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 40, "eco_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 47, "valve_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_xalsoe3m_valve_state_dp47, sizeof(kEnum__TZE204_xalsoe3m_valve_state_dp47)/sizeof(kEnum__TZE204_xalsoe3m_valve_state_dp47[0]) },
    { 50, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 68, "programming_mode", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "max_temperature_limit", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "deadzone_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_xalsoe3m{ kEntries__TZE204_xalsoe3m, 14 };
constexpr FzConverter kFzDp__TZE204_xalsoe3m{
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
    .user_config       = &kMap__TZE204_xalsoe3m,
};
const FzConverter* const kFz__TZE204_xalsoe3m[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_xalsoe3m,
};
constexpr TzConverter kTzDp__TZE204_xalsoe3m{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_xalsoe3m,
};
const TzConverter* const kTz__TZE204_xalsoe3m[] = { &kTzDp__TZE204_xalsoe3m };
constexpr const char* kM__TZE204_xalsoe3m[] = { "TS0601" };
constexpr const char* kN__TZE204_xalsoe3m[] = { "_TZE204_xalsoe3m" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE204_xalsoe3m_4[] = {"IN", "AL", "OU"};
constexpr const char* kAutoOpts__TZE204_xalsoe3m_5[] = {"close", "open"};
constexpr Expose kAutoExposes__TZE204_xalsoe3m[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"system_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"eco_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"sensor", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_xalsoe3m_4, 3, ExposeCategory::Config},
    {"valve_state", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_xalsoe3m_5, 2},
    {"min_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 0, 20, 0},
    {"max_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 20, 50, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 0, 50, 1},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -9, 9, 1},
    {"max_temperature_limit", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 25, 70, 1},
    {"deadzone_temperature", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 5, 1},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_xalsoe3m{
    .zigbee_models=kM__TZE204_xalsoe3m,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_xalsoe3m,.manufacturer_names_count=1,
    .model="TS0601__TZE204_xalsoe3m",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_xalsoe3m,.exposes_count=sizeof(kAutoExposes__TZE204_xalsoe3m)/sizeof(kAutoExposes__TZE204_xalsoe3m[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_xalsoe3m,
    .from_zigbee_count=sizeof(kFz__TZE204_xalsoe3m)/sizeof(kFz__TZE204_xalsoe3m[0]),
    .to_zigbee=kTz__TZE204_xalsoe3m,
    .to_zigbee_count=sizeof(kTz__TZE204_xalsoe3m)/sizeof(kTz__TZE204_xalsoe3m[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
