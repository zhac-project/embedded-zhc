// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_v1jqz5cy).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_v1jqz5cy[] = {
    { 1, "tds", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 2, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 7, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 11, "ec", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "orp", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "free_chlorine", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 105, "backlightvalue", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "ph_max", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "ph_min", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 108, "ec_max", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "ec_min", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 110, "orp_max", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "orp_min", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 112, "free_chlorine_max", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 113, "free_chlorine_min", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 117, "salinity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_v1jqz5cy{ kEntries__TZE200_v1jqz5cy, 16 };
constexpr FzConverter kFzDp__TZE200_v1jqz5cy{
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
    .user_config       = &kMap__TZE200_v1jqz5cy,
};
const FzConverter* const kFz__TZE200_v1jqz5cy[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_v1jqz5cy,
};
constexpr TzConverter kTzDp__TZE200_v1jqz5cy{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_v1jqz5cy,
};
const TzConverter* const kTz__TZE200_v1jqz5cy[] = { &kTzDp__TZE200_v1jqz5cy };
constexpr const char* kM__TZE200_v1jqz5cy[] = { "TS0601" };
constexpr const char* kN__TZE200_v1jqz5cy[] = { "_TZE200_v1jqz5cy" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_v1jqz5cy[] = {
    {"Tuya","YK-S03"},
};
extern const PreparedDefinition kDefGen__TZE200_v1jqz5cy{
    .zigbee_models=kM__TZE200_v1jqz5cy,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_v1jqz5cy,.manufacturer_names_count=1,
    .model="TS0601__TZE200_v1jqz5cy",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_v1jqz5cy, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_v1jqz5cy)/sizeof(kWhiteLabels_Gen_TZE200_v1jqz5cy[0]),
    .from_zigbee=kFz__TZE200_v1jqz5cy,
    .from_zigbee_count=sizeof(kFz__TZE200_v1jqz5cy)/sizeof(kFz__TZE200_v1jqz5cy[0]),
    .to_zigbee=kTz__TZE200_v1jqz5cy,
    .to_zigbee_count=sizeof(kTz__TZE200_v1jqz5cy)/sizeof(kTz__TZE200_v1jqz5cy[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
