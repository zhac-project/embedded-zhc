// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_lhqtjwax).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_lhqtjwax_temperature_unit_dp9[] = {
    { 0, "celsius" },
    { 1, "fahrenheit" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_lhqtjwax[] = {
    { 1, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 2, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "temperature_unit", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_lhqtjwax_temperature_unit_dp9, sizeof(kEnum__TZE200_lhqtjwax_temperature_unit_dp9)/sizeof(kEnum__TZE200_lhqtjwax_temperature_unit_dp9[0]) },
    { 23, "temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 24, "humidity_calibration", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_lhqtjwax{ kEntries__TZE200_lhqtjwax, 6 };
constexpr FzConverter kFzDp__TZE200_lhqtjwax{
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
    .user_config       = &kMap__TZE200_lhqtjwax,
};
const FzConverter* const kFz__TZE200_lhqtjwax[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_lhqtjwax,
};
constexpr TzConverter kTzDp__TZE200_lhqtjwax{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_lhqtjwax,
};
const TzConverter* const kTz__TZE200_lhqtjwax[] = { &kTzDp__TZE200_lhqtjwax };
constexpr const char* kM__TZE200_lhqtjwax[] = { "TS0601" };
constexpr const char* kN__TZE200_lhqtjwax[] = { "_TZE200_lhqtjwax" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_lhqtjwax{
    .zigbee_models=kM__TZE200_lhqtjwax,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_lhqtjwax,.manufacturer_names_count=1,
    .model="TS0601__TZE200_lhqtjwax",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_lhqtjwax,
    .from_zigbee_count=sizeof(kFz__TZE200_lhqtjwax)/sizeof(kFz__TZE200_lhqtjwax[0]),
    .to_zigbee=kTz__TZE200_lhqtjwax,
    .to_zigbee_count=sizeof(kTz__TZE200_lhqtjwax)/sizeof(kTz__TZE200_lhqtjwax[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
