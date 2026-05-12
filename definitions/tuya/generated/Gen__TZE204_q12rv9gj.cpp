// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_q12rv9gj).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_q12rv9gj_system_mode_dp2[] = {
    { 0, "cool" },
    { 1, "heat" },
    { 2, "fan_only" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_q12rv9gj_fan_mode_dp28[] = {
    { 0, "auto" },
    { 1, "high" },
    { 2, "medium" },
    { 3, "low" },
    { 4, "off" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_q12rv9gj_manual_mode_dp101[] = {
    { 0, "Auto" },
    { 1, "Manual" },
    { 2, "Tempoary" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_q12rv9gj[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "system_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_q12rv9gj_system_mode_dp2, sizeof(kEnum__TZE204_q12rv9gj_system_mode_dp2)/sizeof(kEnum__TZE204_q12rv9gj_system_mode_dp2[0]) },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 19, "max_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 18, "max_temperature_f", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 20, "min_temperature_f", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 21, "local_temperature_f", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 26, "min_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 102, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 103, "local_temperature_calibration_f", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "deadzone_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "deadzone_temperature_f", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 28, "fan_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_q12rv9gj_fan_mode_dp28, sizeof(kEnum__TZE204_q12rv9gj_fan_mode_dp28)/sizeof(kEnum__TZE204_q12rv9gj_fan_mode_dp28[0]) },
    { 40, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 44, "current_heating_setpoint_f", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 46, "temperature_scale", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "manual_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_q12rv9gj_manual_mode_dp101, sizeof(kEnum__TZE204_q12rv9gj_manual_mode_dp101)/sizeof(kEnum__TZE204_q12rv9gj_manual_mode_dp101[0]) },
    { 113, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_q12rv9gj{ kEntries__TZE204_q12rv9gj, 19 };
constexpr FzConverter kFzDp__TZE204_q12rv9gj{
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
    .user_config       = &kMap__TZE204_q12rv9gj,
};
const FzConverter* const kFz__TZE204_q12rv9gj[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_q12rv9gj,
};
constexpr TzConverter kTzDp__TZE204_q12rv9gj{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_q12rv9gj,
};
const TzConverter* const kTz__TZE204_q12rv9gj[] = { &kTzDp__TZE204_q12rv9gj };
constexpr const char* kM__TZE204_q12rv9gj[] = { "TS0601" };
constexpr const char* kN__TZE204_q12rv9gj[] = { "_TZE204_q12rv9gj" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_q12rv9gj{
    .zigbee_models=kM__TZE204_q12rv9gj,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_q12rv9gj,.manufacturer_names_count=1,
    .model="TS0601__TZE204_q12rv9gj",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_q12rv9gj,
    .from_zigbee_count=sizeof(kFz__TZE204_q12rv9gj)/sizeof(kFz__TZE204_q12rv9gj[0]),
    .to_zigbee=kTz__TZE204_q12rv9gj,
    .to_zigbee_count=sizeof(kTz__TZE204_q12rv9gj)/sizeof(kTz__TZE204_q12rv9gj[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
