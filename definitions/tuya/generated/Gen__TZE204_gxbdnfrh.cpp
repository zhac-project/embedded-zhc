// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_gxbdnfrh).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_gxbdnfrh_power_on_behavior_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "memory" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_gxbdnfrh_indicator_mode_dp15[] = {
    { 0, "none" },
    { 1, "relay" },
    { 2, "pos" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_gxbdnfrh[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 5, "state_l5", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 6, "state_l6", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 14, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_gxbdnfrh_power_on_behavior_dp14, sizeof(kEnum__TZE204_gxbdnfrh_power_on_behavior_dp14)/sizeof(kEnum__TZE204_gxbdnfrh_power_on_behavior_dp14[0]) },
    { 15, "indicator_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_gxbdnfrh_indicator_mode_dp15, sizeof(kEnum__TZE204_gxbdnfrh_indicator_mode_dp15)/sizeof(kEnum__TZE204_gxbdnfrh_indicator_mode_dp15[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_gxbdnfrh{ kEntries__TZE204_gxbdnfrh, 8 };
constexpr FzConverter kFzDp__TZE204_gxbdnfrh{
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
    .user_config       = &kMap__TZE204_gxbdnfrh,
};
const FzConverter* const kFz__TZE204_gxbdnfrh[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_gxbdnfrh,
};
constexpr TzConverter kTzDp__TZE204_gxbdnfrh{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_gxbdnfrh,
};
const TzConverter* const kTz__TZE204_gxbdnfrh[] = { &kTzDp__TZE204_gxbdnfrh };
constexpr const char* kM__TZE204_gxbdnfrh[] = { "TS0601" };
constexpr const char* kN__TZE204_gxbdnfrh[] = { "_TZE204_gxbdnfrh" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_gxbdnfrh[] = {
    {"Mercator Ikuü","SSW06G"},
    {"Nova Digital","NTZB-04-W-B"},
    {"Nova Digital","SYZB-6W"},
    {"Nova Digital","FZB-6"},
    {"Nova Digital","SA-6"},
    {"Ekaza","EKAT-T3074-6WZ"},
};
extern const PreparedDefinition kDefGen__TZE204_gxbdnfrh{
    .zigbee_models=kM__TZE204_gxbdnfrh,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_gxbdnfrh,.manufacturer_names_count=1,
    .model="TS0601__TZE204_gxbdnfrh",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_gxbdnfrh, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_gxbdnfrh)/sizeof(kWhiteLabels_Gen_TZE204_gxbdnfrh[0]),
    .from_zigbee=kFz__TZE204_gxbdnfrh,
    .from_zigbee_count=sizeof(kFz__TZE204_gxbdnfrh)/sizeof(kFz__TZE204_gxbdnfrh[0]),
    .to_zigbee=kTz__TZE204_gxbdnfrh,
    .to_zigbee_count=sizeof(kTz__TZE204_gxbdnfrh)/sizeof(kTz__TZE204_gxbdnfrh[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
