// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_7yyuo8sr).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_7yyuo8sr_liquid_state_dp1[] = {
    { 1, "low" },
    { 0, "normal" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_7yyuo8sr[] = {
    { 1, "liquid_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_7yyuo8sr_liquid_state_dp1, sizeof(kEnum__TZE204_7yyuo8sr_liquid_state_dp1)/sizeof(kEnum__TZE204_7yyuo8sr_liquid_state_dp1[0]) },
    { 2, "liquid_depth", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 22, "liquid_level_percent", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "max_set", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "mini_set", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 19, "installation_height", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 21, "liquid_depth_max", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_7yyuo8sr{ kEntries__TZE204_7yyuo8sr, 7 };
constexpr FzConverter kFzDp__TZE204_7yyuo8sr{
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
    .user_config       = &kMap__TZE204_7yyuo8sr,
};
const FzConverter* const kFz__TZE204_7yyuo8sr[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_7yyuo8sr,
};
constexpr TzConverter kTzDp__TZE204_7yyuo8sr{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_7yyuo8sr,
};
const TzConverter* const kTz__TZE204_7yyuo8sr[] = { &kTzDp__TZE204_7yyuo8sr };
constexpr const char* kM__TZE204_7yyuo8sr[] = { "TS0601" };
constexpr const char* kN__TZE204_7yyuo8sr[] = { "_TZE204_7yyuo8sr" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_7yyuo8sr[] = {
    {"Tuya","872WZ"},
};
extern const PreparedDefinition kDefGen__TZE204_7yyuo8sr{
    .zigbee_models=kM__TZE204_7yyuo8sr,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_7yyuo8sr,.manufacturer_names_count=1,
    .model="TS0601__TZE204_7yyuo8sr",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_7yyuo8sr, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_7yyuo8sr)/sizeof(kWhiteLabels_Gen_TZE204_7yyuo8sr[0]),
    .from_zigbee=kFz__TZE204_7yyuo8sr,
    .from_zigbee_count=sizeof(kFz__TZE204_7yyuo8sr)/sizeof(kFz__TZE204_7yyuo8sr[0]),
    .to_zigbee=kTz__TZE204_7yyuo8sr,
    .to_zigbee_count=sizeof(kTz__TZE204_7yyuo8sr)/sizeof(kTz__TZE204_7yyuo8sr[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
