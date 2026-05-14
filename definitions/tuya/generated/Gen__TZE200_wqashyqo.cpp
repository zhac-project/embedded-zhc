// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_wqashyqo).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_wqashyqo_water_warning_dp1[] = {
    { 0, "none" },
    { 1, "alarm" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_wqashyqo_temperature_unit_dp106[] = {
    { 0, "celsius" },
    { 1, "fahrenheit" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_wqashyqo[] = {
    { 1, "water_warning", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_wqashyqo_water_warning_dp1, sizeof(kEnum__TZE200_wqashyqo_water_warning_dp1)/sizeof(kEnum__TZE200_wqashyqo_water_warning_dp1[0]) },
    { 103, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 109, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "soil_moisture", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 108, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "temperature_unit", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_wqashyqo_temperature_unit_dp106, sizeof(kEnum__TZE200_wqashyqo_temperature_unit_dp106)/sizeof(kEnum__TZE200_wqashyqo_temperature_unit_dp106[0]) },
    { 104, "temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "humidity_calibration", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 102, "soil_calibration", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 111, "temperature_sampling", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 112, "soil_sampling", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 110, "soil_warning", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_wqashyqo{ kEntries__TZE200_wqashyqo, 12 };
constexpr FzConverter kFzDp__TZE200_wqashyqo{
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
    .user_config       = &kMap__TZE200_wqashyqo,
};
const FzConverter* const kFz__TZE200_wqashyqo[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_wqashyqo,
};
constexpr TzConverter kTzDp__TZE200_wqashyqo{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_wqashyqo,
};
const TzConverter* const kTz__TZE200_wqashyqo[] = { &kTzDp__TZE200_wqashyqo };
constexpr const char* kM__TZE200_wqashyqo[] = { "TS0601" };
constexpr const char* kN__TZE200_wqashyqo[] = { "_TZE200_wqashyqo" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_wqashyqo{
    .zigbee_models=kM__TZE200_wqashyqo,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_wqashyqo,.manufacturer_names_count=1,
    .model="ZG-303Z",.vendor="HOBEIAN",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_wqashyqo,
    .from_zigbee_count=sizeof(kFz__TZE200_wqashyqo)/sizeof(kFz__TZE200_wqashyqo[0]),
    .to_zigbee=kTz__TZE200_wqashyqo,
    .to_zigbee_count=sizeof(kTz__TZE200_wqashyqo)/sizeof(kTz__TZE200_wqashyqo[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
