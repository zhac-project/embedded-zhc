// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_mrffaamu).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_mrffaamu_over_voltage_setting_dp102[] = {
    { 0, "closed" },
    { 1, "alarm" },
    { 2, "trip" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_mrffaamu_under_voltage_setting_dp103[] = {
    { 0, "closed" },
    { 1, "alarm" },
    { 2, "trip" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_mrffaamu_over_current_setting_dp104[] = {
    { 0, "closed" },
    { 1, "alarm" },
    { 2, "trip" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_mrffaamu_over_power_setting_dp105[] = {
    { 0, "closed" },
    { 1, "alarm" },
    { 2, "trip" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_mrffaamu_temperature_setting_dp107[] = {
    { 0, "closed" },
    { 1, "alarm" },
    { 2, "trip" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_mrffaamu_last_event_dp110[] = {
    { 0, "normal" },
    { 1, "trip_over_current" },
    { 2, "trip_over_power" },
    { 3, "trip_over_temperature" },
    { 4, "trip_voltage_1" },
    { 5, "trip_voltage_2" },
    { 6, "alarm_over_current" },
    { 7, "alarm_over_power" },
    { 8, "alarm_over_temperature" },
    { 9, "alarm_voltage_1" },
    { 10, "alarm_voltage_2" },
    { 11, "remote_on" },
    { 12, "remote_off" },
    { 13, "manual_on" },
    { 14, "manual_off" },
    { 15, "value_15" },
    { 16, "value_16" },
    { 17, "factory_reset" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_mrffaamu[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 16, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "over_voltage_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_mrffaamu_over_voltage_setting_dp102, sizeof(kEnum__TZE204_mrffaamu_over_voltage_setting_dp102)/sizeof(kEnum__TZE204_mrffaamu_over_voltage_setting_dp102[0]) },
    { 103, "under_voltage_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_mrffaamu_under_voltage_setting_dp103, sizeof(kEnum__TZE204_mrffaamu_under_voltage_setting_dp103)/sizeof(kEnum__TZE204_mrffaamu_under_voltage_setting_dp103[0]) },
    { 104, "over_current_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_mrffaamu_over_current_setting_dp104, sizeof(kEnum__TZE204_mrffaamu_over_current_setting_dp104)/sizeof(kEnum__TZE204_mrffaamu_over_current_setting_dp104[0]) },
    { 105, "over_power_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_mrffaamu_over_power_setting_dp105, sizeof(kEnum__TZE204_mrffaamu_over_power_setting_dp105)/sizeof(kEnum__TZE204_mrffaamu_over_power_setting_dp105[0]) },
    { 107, "temperature_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_mrffaamu_temperature_setting_dp107, sizeof(kEnum__TZE204_mrffaamu_temperature_setting_dp107)/sizeof(kEnum__TZE204_mrffaamu_temperature_setting_dp107[0]) },
    { 110, "last_event", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_mrffaamu_last_event_dp110, sizeof(kEnum__TZE204_mrffaamu_last_event_dp110)/sizeof(kEnum__TZE204_mrffaamu_last_event_dp110[0]) },
    { 112, "clear_fault", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 113, "factory_reset", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 114, "current_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 115, "over_voltage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 116, "under_voltage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 118, "temperature_threshold", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 119, "over_power_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 131, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_mrffaamu{ kEntries__TZE204_mrffaamu, 16 };
constexpr FzConverter kFzDp__TZE204_mrffaamu{
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
    .user_config       = &kMap__TZE204_mrffaamu,
};
const FzConverter* const kFz__TZE204_mrffaamu[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_mrffaamu,
};
constexpr TzConverter kTzDp__TZE204_mrffaamu{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_mrffaamu,
};
const TzConverter* const kTz__TZE204_mrffaamu[] = { &kTzDp__TZE204_mrffaamu };
constexpr const char* kM__TZE204_mrffaamu[] = { "TS0601" };
constexpr const char* kN__TZE204_mrffaamu[] = { "_TZE204_mrffaamu" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_mrffaamu{
    .zigbee_models=kM__TZE204_mrffaamu,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_mrffaamu,.manufacturer_names_count=1,
    .model="TS0601__TZE204_mrffaamu",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_mrffaamu,
    .from_zigbee_count=sizeof(kFz__TZE204_mrffaamu)/sizeof(kFz__TZE204_mrffaamu[0]),
    .to_zigbee=kTz__TZE204_mrffaamu,
    .to_zigbee_count=sizeof(kTz__TZE204_mrffaamu)/sizeof(kTz__TZE204_mrffaamu[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
