// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_w6n8jeuu).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_w6n8jeuu_battery_state_dp3[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_w6n8jeuu_temperature_unit_dp9[] = {
    { 0, "celsius" },
    { 1, "fahrenheit" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_w6n8jeuu_temperature_alarm_dp14[] = {
    { 0, "lower_alarm" },
    { 1, "upper_alarm" },
    { 2, "cancel" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_w6n8jeuu_humidity_alarm_dp15[] = {
    { 0, "lower_alarm" },
    { 1, "upper_alarm" },
    { 2, "cancel" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_w6n8jeuu[] = {
    { 1, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 2, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_w6n8jeuu_battery_state_dp3, sizeof(kEnum__TZE200_w6n8jeuu_battery_state_dp3)/sizeof(kEnum__TZE200_w6n8jeuu_battery_state_dp3[0]) },
    { 4, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "temperature_unit", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_w6n8jeuu_temperature_unit_dp9, sizeof(kEnum__TZE200_w6n8jeuu_temperature_unit_dp9)/sizeof(kEnum__TZE200_w6n8jeuu_temperature_unit_dp9[0]) },
    { 10, "max_temperature_alarm", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 11, "min_temperature_alarm", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 12, "max_humidity_alarm", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 13, "min_humidity_alarm", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "temperature_alarm", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_w6n8jeuu_temperature_alarm_dp14, sizeof(kEnum__TZE200_w6n8jeuu_temperature_alarm_dp14)/sizeof(kEnum__TZE200_w6n8jeuu_temperature_alarm_dp14[0]) },
    { 15, "humidity_alarm", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_w6n8jeuu_humidity_alarm_dp15, sizeof(kEnum__TZE200_w6n8jeuu_humidity_alarm_dp15)/sizeof(kEnum__TZE200_w6n8jeuu_humidity_alarm_dp15[0]) },
    { 17, "temperature_periodic_report", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "humidity_periodic_report", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 19, "temperature_sensitivity", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 20, "humidity_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_w6n8jeuu{ kEntries__TZE200_w6n8jeuu, 15 };
constexpr FzConverter kFzDp__TZE200_w6n8jeuu{
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
    .user_config       = &kMap__TZE200_w6n8jeuu,
};
const FzConverter* const kFz__TZE200_w6n8jeuu[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_w6n8jeuu,
};
constexpr TzConverter kTzDp__TZE200_w6n8jeuu{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_w6n8jeuu,
};
const TzConverter* const kTz__TZE200_w6n8jeuu[] = { &kTzDp__TZE200_w6n8jeuu };
constexpr const char* kM__TZE200_w6n8jeuu[] = { "TS0601" };
constexpr const char* kN__TZE200_w6n8jeuu[] = { "_TZE200_w6n8jeuu" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_w6n8jeuu{
    .zigbee_models=kM__TZE200_w6n8jeuu,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_w6n8jeuu,.manufacturer_names_count=1,
    .model="TS0601__TZE200_w6n8jeuu",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_w6n8jeuu,
    .from_zigbee_count=sizeof(kFz__TZE200_w6n8jeuu)/sizeof(kFz__TZE200_w6n8jeuu[0]),
    .to_zigbee=kTz__TZE200_w6n8jeuu,
    .to_zigbee_count=sizeof(kTz__TZE200_w6n8jeuu)/sizeof(kTz__TZE200_w6n8jeuu[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
