// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_clrdrnya).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_clrdrnya_breaker_mode_dp107[] = {
    { 0, "standard" },
    { 1, "local" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_clrdrnya_breaker_status_dp108[] = {
    { 0, "OFF" },
    { 1, "ON" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_clrdrnya_status_indication_dp109[] = {
    { 0, "OFF" },
    { 1, "ON" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_clrdrnya_breaker_polarity_dp111[] = {
    { 0, "NC" },
    { 1, "NO" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_clrdrnya_sensor_dp115[] = {
    { 0, "on" },
    { 1, "off" },
    { 2, "occupied" },
    { 3, "unoccupied" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_clrdrnya[] = {
    { 1, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "radar_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "shield_range", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 4, "detection_range", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 6, "equipment_status", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "target_distance", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 101, "entry_filter_time", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 102, "departure_delay", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "cline", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "illuminance", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 105, "entry_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "entry_distance_indentation", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 107, "breaker_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_clrdrnya_breaker_mode_dp107, sizeof(kEnum__TZE204_clrdrnya_breaker_mode_dp107)/sizeof(kEnum__TZE204_clrdrnya_breaker_mode_dp107[0]) },
    { 108, "breaker_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_clrdrnya_breaker_status_dp108, sizeof(kEnum__TZE204_clrdrnya_breaker_status_dp108)/sizeof(kEnum__TZE204_clrdrnya_breaker_status_dp108[0]) },
    { 109, "status_indication", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_clrdrnya_status_indication_dp109, sizeof(kEnum__TZE204_clrdrnya_status_indication_dp109)/sizeof(kEnum__TZE204_clrdrnya_status_indication_dp109[0]) },
    { 110, "illuminance_threshold", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 111, "breaker_polarity", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_clrdrnya_breaker_polarity_dp111, sizeof(kEnum__TZE204_clrdrnya_breaker_polarity_dp111)/sizeof(kEnum__TZE204_clrdrnya_breaker_polarity_dp111[0]) },
    { 112, "block_time", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 113, "parameter_setting_result", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 114, "factory_parameters", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 115, "sensor", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_clrdrnya_sensor_dp115, sizeof(kEnum__TZE204_clrdrnya_sensor_dp115)/sizeof(kEnum__TZE204_clrdrnya_sensor_dp115[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_clrdrnya{ kEntries__TZE204_clrdrnya, 21 };
constexpr FzConverter kFzDp__TZE204_clrdrnya{
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
    .user_config       = &kMap__TZE204_clrdrnya,
};
const FzConverter* const kFz__TZE204_clrdrnya[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_clrdrnya,
};
constexpr TzConverter kTzDp__TZE204_clrdrnya{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_clrdrnya,
};
const TzConverter* const kTz__TZE204_clrdrnya[] = { &kTzDp__TZE204_clrdrnya };
constexpr const char* kM__TZE204_clrdrnya[] = { "TS0601" };
constexpr const char* kN__TZE204_clrdrnya[] = { "_TZE204_clrdrnya" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_clrdrnya{
    .zigbee_models=kM__TZE204_clrdrnya,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_clrdrnya,.manufacturer_names_count=1,
    .model="TS0601__TZE204_clrdrnya",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_clrdrnya,
    .from_zigbee_count=sizeof(kFz__TZE204_clrdrnya)/sizeof(kFz__TZE204_clrdrnya[0]),
    .to_zigbee=kTz__TZE204_clrdrnya,
    .to_zigbee_count=sizeof(kTz__TZE204_clrdrnya)/sizeof(kTz__TZE204_clrdrnya[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
