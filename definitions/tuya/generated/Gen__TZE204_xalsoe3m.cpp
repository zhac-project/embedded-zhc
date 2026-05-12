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
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_xalsoe3m{
    .zigbee_models=kM__TZE204_xalsoe3m,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_xalsoe3m,.manufacturer_names_count=1,
    .model="TS0601__TZE204_xalsoe3m",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_xalsoe3m,
    .from_zigbee_count=sizeof(kFz__TZE204_xalsoe3m)/sizeof(kFz__TZE204_xalsoe3m[0]),
    .to_zigbee=kTz__TZE204_xalsoe3m,
    .to_zigbee_count=sizeof(kTz__TZE204_xalsoe3m)/sizeof(kTz__TZE204_xalsoe3m[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
