// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_ksz749x8).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_ksz749x8_battery_state_dp3[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_ksz749x8_temperature_unit_dp9[] = {
    { 0, "celsius" },
    { 1, "fahrenheit" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_ksz749x8[] = {
    { 1, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 2, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_ksz749x8_battery_state_dp3, sizeof(kEnum__TZE204_ksz749x8_battery_state_dp3)/sizeof(kEnum__TZE204_ksz749x8_battery_state_dp3[0]) },
    { 9, "temperature_unit", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_ksz749x8_temperature_unit_dp9, sizeof(kEnum__TZE204_ksz749x8_temperature_unit_dp9)/sizeof(kEnum__TZE204_ksz749x8_temperature_unit_dp9[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_ksz749x8{ kEntries__TZE204_ksz749x8, 4 };
constexpr FzConverter kFzDp__TZE204_ksz749x8{
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
    .user_config       = &kMap__TZE204_ksz749x8,
};
const FzConverter* const kFz__TZE204_ksz749x8[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_ksz749x8,
};
constexpr TzConverter kTzDp__TZE204_ksz749x8{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_ksz749x8,
};
const TzConverter* const kTz__TZE204_ksz749x8[] = { &kTzDp__TZE204_ksz749x8 };
constexpr const char* kM__TZE204_ksz749x8[] = { "TS0601" };
constexpr const char* kN__TZE204_ksz749x8[] = { "_TZE204_ksz749x8" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_ksz749x8[] = {
    {"Tuya","ZTH01"},
    {"Tuya","SZTH02"},
    {"Tuya","ZTH02"},
    {"Tuya","ZTH05"},
    {"Tuya","ZTH08-E"},
    {"Tuya","ZTH08"},
};
extern const PreparedDefinition kDefGen__TZE204_ksz749x8{
    .zigbee_models=kM__TZE204_ksz749x8,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_ksz749x8,.manufacturer_names_count=1,
    .model="TS0601__TZE204_ksz749x8",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_ksz749x8, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_ksz749x8)/sizeof(kWhiteLabels_Gen_TZE204_ksz749x8[0]),
    .from_zigbee=kFz__TZE204_ksz749x8,
    .from_zigbee_count=sizeof(kFz__TZE204_ksz749x8)/sizeof(kFz__TZE204_ksz749x8[0]),
    .to_zigbee=kTz__TZE204_ksz749x8,
    .to_zigbee_count=sizeof(kTz__TZE204_ksz749x8)/sizeof(kTz__TZE204_ksz749x8[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
