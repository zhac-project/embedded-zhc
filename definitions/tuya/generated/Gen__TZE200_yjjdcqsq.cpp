// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_yjjdcqsq).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yjjdcqsq_battery_state_dp3[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_yjjdcqsq_temperature_unit_dp9[] = {
    { 0, "celsius" },
    { 1, "fahrenheit" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_yjjdcqsq[] = {
    { 1, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 2, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yjjdcqsq_battery_state_dp3, sizeof(kEnum__TZE200_yjjdcqsq_battery_state_dp3)/sizeof(kEnum__TZE200_yjjdcqsq_battery_state_dp3[0]) },
    { 9, "temperature_unit", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_yjjdcqsq_temperature_unit_dp9, sizeof(kEnum__TZE200_yjjdcqsq_temperature_unit_dp9)/sizeof(kEnum__TZE200_yjjdcqsq_temperature_unit_dp9[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_yjjdcqsq{ kEntries__TZE200_yjjdcqsq, 4 };
constexpr FzConverter kFzDp__TZE200_yjjdcqsq{
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
    .user_config       = &kMap__TZE200_yjjdcqsq,
};
const FzConverter* const kFz__TZE200_yjjdcqsq[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_yjjdcqsq,
};
constexpr TzConverter kTzDp__TZE200_yjjdcqsq{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_yjjdcqsq,
};
const TzConverter* const kTz__TZE200_yjjdcqsq[] = { &kTzDp__TZE200_yjjdcqsq };
constexpr const char* kM__TZE200_yjjdcqsq[] = { "TS0601" };
constexpr const char* kN__TZE200_yjjdcqsq[] = { "_TZE200_yjjdcqsq" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_yjjdcqsq[] = {
    {"Tuya","ZTH01"},
    {"Tuya","SZTH02"},
    {"Tuya","ZTH02"},
    {"Tuya","ZTH05"},
    {"Tuya","ZTH08-E"},
    {"Tuya","ZTH08"},
};
extern const PreparedDefinition kDefGen__TZE200_yjjdcqsq{
    .zigbee_models=kM__TZE200_yjjdcqsq,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_yjjdcqsq,.manufacturer_names_count=1,
    .model="TS0601__TZE200_yjjdcqsq",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_yjjdcqsq, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_yjjdcqsq)/sizeof(kWhiteLabels_Gen_TZE200_yjjdcqsq[0]),
    .from_zigbee=kFz__TZE200_yjjdcqsq,
    .from_zigbee_count=sizeof(kFz__TZE200_yjjdcqsq)/sizeof(kFz__TZE200_yjjdcqsq[0]),
    .to_zigbee=kTz__TZE200_yjjdcqsq,
    .to_zigbee_count=sizeof(kTz__TZE200_yjjdcqsq)/sizeof(kTz__TZE200_yjjdcqsq[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
