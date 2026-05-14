// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_lbhh5o6z).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_lbhh5o6z[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_lbhh5o6z{ kEntries__TZE284_lbhh5o6z, 4 };
constexpr FzConverter kFzDp__TZE284_lbhh5o6z{
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
    .user_config       = &kMap__TZE284_lbhh5o6z,
};
const FzConverter* const kFz__TZE284_lbhh5o6z[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_lbhh5o6z,
};
constexpr TzConverter kTzDp__TZE284_lbhh5o6z{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_lbhh5o6z,
};
const TzConverter* const kTz__TZE284_lbhh5o6z[] = { &kTzDp__TZE284_lbhh5o6z };
constexpr const char* kM__TZE284_lbhh5o6z[] = { "TS0601" };
constexpr const char* kN__TZE284_lbhh5o6z[] = { "_TZE284_lbhh5o6z" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE284_lbhh5o6z[] = {
    {"ZYXH","TY-04Z"},
    {"AVATTO","WSMD-4"},
    {"AVATTO","ZWSMD-4"},
    {"Tuya","MG-ZG04W"},
    {"Norklmes","MKS-CM-W5"},
    {"Somgoms","ZSQB-SMB-ZB"},
    {"Moes","WS-EUB1-ZG"},
    {"AVATTO","ZGB-WS-EU"},
};
extern const PreparedDefinition kDefGen__TZE284_lbhh5o6z{
    .zigbee_models=kM__TZE284_lbhh5o6z,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_lbhh5o6z,.manufacturer_names_count=1,
    .model="TS0601__TZE284_lbhh5o6z",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE284_lbhh5o6z, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE284_lbhh5o6z)/sizeof(kWhiteLabels_Gen_TZE284_lbhh5o6z[0]),
    .from_zigbee=kFz__TZE284_lbhh5o6z,
    .from_zigbee_count=sizeof(kFz__TZE284_lbhh5o6z)/sizeof(kFz__TZE284_lbhh5o6z[0]),
    .to_zigbee=kTz__TZE284_lbhh5o6z,
    .to_zigbee_count=sizeof(kTz__TZE284_lbhh5o6z)/sizeof(kTz__TZE284_lbhh5o6z[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
