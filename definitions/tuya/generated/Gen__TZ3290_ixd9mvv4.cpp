// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0049 / _TZ3290_ixd9mvv4).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZ3290_ixd9mvv4_rain_sensor_status_dp101[] = {
    { 0, "rain" },
    { 1, "no_rain" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZ3290_ixd9mvv4_battery_state_dp103[] = {
    { 0, "no_charge" },
    { 1, "charging" },
    { 2, "charged" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZ3290_ixd9mvv4[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "error_status", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 11, "countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "rain_sensor_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZ3290_ixd9mvv4_rain_sensor_status_dp101, sizeof(kEnum__TZ3290_ixd9mvv4_rain_sensor_status_dp101)/sizeof(kEnum__TZ3290_ixd9mvv4_rain_sensor_status_dp101[0]) },
    { 102, "rain_sensor_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 103, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZ3290_ixd9mvv4_battery_state_dp103, sizeof(kEnum__TZ3290_ixd9mvv4_battery_state_dp103)/sizeof(kEnum__TZ3290_ixd9mvv4_battery_state_dp103[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZ3290_ixd9mvv4{ kEntries__TZ3290_ixd9mvv4, 6 };
constexpr FzConverter kFzDp__TZ3290_ixd9mvv4{
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
    .user_config       = &kMap__TZ3290_ixd9mvv4,
};
const FzConverter* const kFz__TZ3290_ixd9mvv4[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZ3290_ixd9mvv4,
};
constexpr TzConverter kTzDp__TZ3290_ixd9mvv4{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZ3290_ixd9mvv4,
};
const TzConverter* const kTz__TZ3290_ixd9mvv4[] = { &kTzDp__TZ3290_ixd9mvv4 };
constexpr const char* kM__TZ3290_ixd9mvv4[] = { "TS0049" };
constexpr const char* kN__TZ3290_ixd9mvv4[] = { "_TZ3290_ixd9mvv4" };
}  // namespace
extern const PreparedDefinition kDefGen__TZ3290_ixd9mvv4{
    .zigbee_models=kM__TZ3290_ixd9mvv4,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZ3290_ixd9mvv4,.manufacturer_names_count=1,
    .model="TS0049__TZ3290_ixd9mvv4",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZ3290_ixd9mvv4,
    .from_zigbee_count=sizeof(kFz__TZ3290_ixd9mvv4)/sizeof(kFz__TZ3290_ixd9mvv4[0]),
    .to_zigbee=kTz__TZ3290_ixd9mvv4,
    .to_zigbee_count=sizeof(kTz__TZ3290_ixd9mvv4)/sizeof(kTz__TZ3290_ixd9mvv4[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
