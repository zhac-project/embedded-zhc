// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_viy9ihs7).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_viy9ihs7_running_state_dp101[] = {
    { 1, "heat" },
    { 0, "idle" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_viy9ihs7_sensor_dp106[] = {
    { 0, "internal" },
    { 1, "external" },
    { 2, "both" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_viy9ihs7_backlight_mode_dp110[] = {
    { 0, "off" },
    { 1, "low" },
    { 2, "medium" },
    { 3, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_viy9ihs7[] = {
    { 2, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 3, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 9, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 11, "faultalarm", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 15, "max_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 19, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_viy9ihs7_running_state_dp101, sizeof(kEnum__TZE200_viy9ihs7_running_state_dp101)/sizeof(kEnum__TZE200_viy9ihs7_running_state_dp101[0]) },
    { 102, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 103, "factory_reset", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 106, "sensor", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_viy9ihs7_sensor_dp106, sizeof(kEnum__TZE200_viy9ihs7_sensor_dp106)/sizeof(kEnum__TZE200_viy9ihs7_sensor_dp106[0]) },
    { 107, "deadzone_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 109, "schedule_weekday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 109, "schedule_holiday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 110, "backlight_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_viy9ihs7_backlight_mode_dp110, sizeof(kEnum__TZE200_viy9ihs7_backlight_mode_dp110)/sizeof(kEnum__TZE200_viy9ihs7_backlight_mode_dp110[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_viy9ihs7{ kEntries__TZE200_viy9ihs7, 14 };
constexpr FzConverter kFzDp__TZE200_viy9ihs7{
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
    .user_config       = &kMap__TZE200_viy9ihs7,
};
const FzConverter* const kFz__TZE200_viy9ihs7[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_viy9ihs7,
};
constexpr TzConverter kTzDp__TZE200_viy9ihs7{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_viy9ihs7,
};
const TzConverter* const kTz__TZE200_viy9ihs7[] = { &kTzDp__TZE200_viy9ihs7 };
constexpr const char* kM__TZE200_viy9ihs7[] = { "TS0601" };
constexpr const char* kN__TZE200_viy9ihs7[] = { "_TZE200_viy9ihs7" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_viy9ihs7{
    .zigbee_models=kM__TZE200_viy9ihs7,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_viy9ihs7,.manufacturer_names_count=1,
    .model="TS0601__TZE200_viy9ihs7",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_viy9ihs7,
    .from_zigbee_count=sizeof(kFz__TZE200_viy9ihs7)/sizeof(kFz__TZE200_viy9ihs7[0]),
    .to_zigbee=kTz__TZE200_viy9ihs7,
    .to_zigbee_count=sizeof(kTz__TZE200_viy9ihs7)/sizeof(kTz__TZE200_viy9ihs7[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
