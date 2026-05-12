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
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_6kijc7nd{
    .zigbee_models=kM__TZE204_6kijc7nd,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_6kijc7nd,.manufacturer_names_count=1,
    .model="TS0601__TZE204_6kijc7nd",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_6kijc7nd,
    .from_zigbee_count=sizeof(kFz__TZE204_6kijc7nd)/sizeof(kFz__TZE204_6kijc7nd[0]),
    .to_zigbee=kTz__TZE204_6kijc7nd,
    .to_zigbee_count=sizeof(kTz__TZE204_6kijc7nd)/sizeof(kTz__TZE204_6kijc7nd[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
