// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya TS0601_switch_4_gang_1 for _TZE204_58of2pfn (z2m v26.115.1 window,
// Tuya DIY-DC-04 4 gang relay board). The family lives in per-manufacturer
// generated copies, identical apart from the manufacturer; this is a hand-written
// copy of _TZE204_mexisfik's for the new member, under z2m's model name.
// z2m-source: tuya.ts #TS0601_switch_4_gang_1.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_58of2pfn[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_58of2pfn{ kEntries__TZE204_58of2pfn, sizeof(kEntries__TZE204_58of2pfn)/sizeof(kEntries__TZE204_58of2pfn[0]) };
constexpr FzConverter kFzDp__TZE204_58of2pfn{
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
    .user_config       = &kMap__TZE204_58of2pfn,
};
const FzConverter* const kFz__TZE204_58of2pfn[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_58of2pfn,
};
constexpr TzConverter kTzDp__TZE204_58of2pfn{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_58of2pfn,
};
const TzConverter* const kTz__TZE204_58of2pfn[] = { &kTzDp__TZE204_58of2pfn };
constexpr const char* kM__TZE204_58of2pfn[] = { "TS0601" };
constexpr const char* kN__TZE204_58of2pfn[] = { "_TZE204_58of2pfn" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr Expose kAutoExposes__TZE204_58of2pfn[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_58of2pfn[] = {
    {"ZYXH","TY-04Z"},
    {"AVATTO","WSMD-4"},
    {"AVATTO","ZWSMD-4"},
    {"Tuya","MG-ZG04W"},
    {"Norklmes","MKS-CM-W5"},
    {"Somgoms","ZSQB-SMB-ZB"},
    {"Moes","WS-EUB1-ZG"},
    {"AVATTO","ZGB-WS-EU"},
    {"Tuya","DIY-DC-04"},
};
extern const PreparedDefinition kDef_TS0601_switch_4_gang_1_58of2pfn{
    .zigbee_models=kM__TZE204_58of2pfn,.zigbee_models_count=sizeof(kM__TZE204_58of2pfn)/sizeof(kM__TZE204_58of2pfn[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_58of2pfn,.manufacturer_names_count=sizeof(kN__TZE204_58of2pfn)/sizeof(kN__TZE204_58of2pfn[0]),
    .model="TS0601_switch_4_gang_1",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_58of2pfn,.exposes_count=sizeof(kAutoExposes__TZE204_58of2pfn)/sizeof(kAutoExposes__TZE204_58of2pfn[0]),
    .white_labels=kWhiteLabels_Gen_TZE204_58of2pfn, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_58of2pfn)/sizeof(kWhiteLabels_Gen_TZE204_58of2pfn[0]),
    .from_zigbee=kFz__TZE204_58of2pfn,
    .from_zigbee_count=sizeof(kFz__TZE204_58of2pfn)/sizeof(kFz__TZE204_58of2pfn[0]),
    .to_zigbee=kTz__TZE204_58of2pfn,
    .to_zigbee_count=sizeof(kTz__TZE204_58of2pfn)/sizeof(kTz__TZE204_58of2pfn[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
