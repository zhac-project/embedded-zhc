// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_awepdiwi).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_awepdiwi_temperature_alarm_dp101[] = {
    { 0, "lower_alarm" },
    { 1, "upper_alarm" },
    { 2, "cancel" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_awepdiwi_humidity_alarm_dp102[] = {
    { 0, "lower_alarm" },
    { 1, "upper_alarm" },
    { 2, "cancel" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_awepdiwi_battery_state_dp14[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_awepdiwi[] = {
    { 101, "temperature_alarm", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_awepdiwi_temperature_alarm_dp101, sizeof(kEnum__TZE284_awepdiwi_temperature_alarm_dp101)/sizeof(kEnum__TZE284_awepdiwi_temperature_alarm_dp101[0]) },
    { 102, "humidity_alarm", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_awepdiwi_humidity_alarm_dp102, sizeof(kEnum__TZE284_awepdiwi_humidity_alarm_dp102)/sizeof(kEnum__TZE284_awepdiwi_humidity_alarm_dp102[0]) },
    { 3, "soil_moisture", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 110, "temperature_f", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 107, "temperature_sensitivity", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 108, "humidity_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "max_temperature_alarm", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 104, "min_temperature_alarm", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 105, "max_humidity_alarm", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "min_humidity_alarm", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "schedule_periodic", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 15, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_awepdiwi_battery_state_dp14, sizeof(kEnum__TZE284_awepdiwi_battery_state_dp14)/sizeof(kEnum__TZE284_awepdiwi_battery_state_dp14[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_awepdiwi{ kEntries__TZE284_awepdiwi, 14 };
constexpr FzConverter kFzDp__TZE284_awepdiwi{
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
    .user_config       = &kMap__TZE284_awepdiwi,
};
const FzConverter* const kFz__TZE284_awepdiwi[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_awepdiwi,
};
constexpr TzConverter kTzDp__TZE284_awepdiwi{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_awepdiwi,
};
const TzConverter* const kTz__TZE284_awepdiwi[] = { &kTzDp__TZE284_awepdiwi };
constexpr const char* kM__TZE284_awepdiwi[] = { "TS0601" };
constexpr const char* kN__TZE284_awepdiwi[] = { "_TZE284_awepdiwi" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_awepdiwi{
    .zigbee_models=kM__TZE284_awepdiwi,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_awepdiwi,.manufacturer_names_count=1,
    .model="TS0601__TZE284_awepdiwi",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_awepdiwi,
    .from_zigbee_count=sizeof(kFz__TZE284_awepdiwi)/sizeof(kFz__TZE284_awepdiwi[0]),
    .to_zigbee=kTz__TZE284_awepdiwi,
    .to_zigbee_count=sizeof(kTz__TZE284_awepdiwi)/sizeof(kTz__TZE284_awepdiwi[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
