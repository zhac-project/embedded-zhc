// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_nvups4nh).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_nvups4nh[] = {
    { 1, "contact", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 2, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 8, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_nvups4nh{ kEntries__TZE200_nvups4nh, 4 };
constexpr FzConverter kFzDp__TZE200_nvups4nh{
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
    .user_config       = &kMap__TZE200_nvups4nh,
};
const FzConverter* const kFz__TZE200_nvups4nh[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_nvups4nh,
};
constexpr TzConverter kTzDp__TZE200_nvups4nh{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_nvups4nh,
};
const TzConverter* const kTz__TZE200_nvups4nh[] = { &kTzDp__TZE200_nvups4nh };
constexpr const char* kM__TZE200_nvups4nh[] = { "TS0601" };
constexpr const char* kN__TZE200_nvups4nh[] = { "_TZE200_nvups4nh" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_nvups4nh[] = {
    {"Aubess","1005005194831629"},
};
extern const PreparedDefinition kDefGen__TZE200_nvups4nh{
    .zigbee_models=kM__TZE200_nvups4nh,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_nvups4nh,.manufacturer_names_count=1,
    .model="TS0601__TZE200_nvups4nh",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_nvups4nh, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_nvups4nh)/sizeof(kWhiteLabels_Gen_TZE200_nvups4nh[0]),
    .from_zigbee=kFz__TZE200_nvups4nh,
    .from_zigbee_count=sizeof(kFz__TZE200_nvups4nh)/sizeof(kFz__TZE200_nvups4nh[0]),
    .to_zigbee=kTz__TZE200_nvups4nh,
    .to_zigbee_count=sizeof(kTz__TZE200_nvups4nh)/sizeof(kTz__TZE200_nvups4nh[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
