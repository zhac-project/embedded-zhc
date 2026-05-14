// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_nvxorhcj).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_nvxorhcj[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 5, "state_l5", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 6, "state_l6", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 101, "state_l7", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "state_l8", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_nvxorhcj{ kEntries__TZE204_nvxorhcj, 8 };
constexpr FzConverter kFzDp__TZE204_nvxorhcj{
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
    .user_config       = &kMap__TZE204_nvxorhcj,
};
const FzConverter* const kFz__TZE204_nvxorhcj[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_nvxorhcj,
};
constexpr TzConverter kTzDp__TZE204_nvxorhcj{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_nvxorhcj,
};
const TzConverter* const kTz__TZE204_nvxorhcj[] = { &kTzDp__TZE204_nvxorhcj };
constexpr const char* kM__TZE204_nvxorhcj[] = { "TS0601" };
constexpr const char* kN__TZE204_nvxorhcj[] = { "_TZE204_nvxorhcj" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_nvxorhcj[] = {
    {"Nova Digital","ZTS-8W-B"},
};
extern const PreparedDefinition kDefGen__TZE204_nvxorhcj{
    .zigbee_models=kM__TZE204_nvxorhcj,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_nvxorhcj,.manufacturer_names_count=1,
    .model="TS0601__TZE204_nvxorhcj",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_nvxorhcj, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_nvxorhcj)/sizeof(kWhiteLabels_Gen_TZE204_nvxorhcj[0]),
    .from_zigbee=kFz__TZE204_nvxorhcj,
    .from_zigbee_count=sizeof(kFz__TZE204_nvxorhcj)/sizeof(kFz__TZE204_nvxorhcj[0]),
    .to_zigbee=kTz__TZE204_nvxorhcj,
    .to_zigbee_count=sizeof(kTz__TZE204_nvxorhcj)/sizeof(kTz__TZE204_nvxorhcj[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
