// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_lbbg34rj).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_lbbg34rj[] = {
    { 12, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "occupancy", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 104, "presence_timeout", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "move_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "breath_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "move_maximum_range", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 110, "move_minimum_range", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "breath_maximum_range", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 112, "breath_minimum_range", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_lbbg34rj{ kEntries__TZE204_lbbg34rj, 9 };
constexpr FzConverter kFzDp__TZE204_lbbg34rj{
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
    .user_config       = &kMap__TZE204_lbbg34rj,
};
const FzConverter* const kFz__TZE204_lbbg34rj[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_lbbg34rj,
};
constexpr TzConverter kTzDp__TZE204_lbbg34rj{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_lbbg34rj,
};
const TzConverter* const kTz__TZE204_lbbg34rj[] = { &kTzDp__TZE204_lbbg34rj };
constexpr const char* kM__TZE204_lbbg34rj[] = { "TS0601" };
constexpr const char* kN__TZE204_lbbg34rj[] = { "_TZE204_lbbg34rj" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_lbbg34rj[] = {
    {"Nova Digital","ZTS-MM"},
};
extern const PreparedDefinition kDefGen__TZE204_lbbg34rj{
    .zigbee_models=kM__TZE204_lbbg34rj,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_lbbg34rj,.manufacturer_names_count=1,
    .model="TS0601__TZE204_lbbg34rj",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_lbbg34rj, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_lbbg34rj)/sizeof(kWhiteLabels_Gen_TZE204_lbbg34rj[0]),
    .from_zigbee=kFz__TZE204_lbbg34rj,
    .from_zigbee_count=sizeof(kFz__TZE204_lbbg34rj)/sizeof(kFz__TZE204_lbbg34rj[0]),
    .to_zigbee=kTz__TZE204_lbbg34rj,
    .to_zigbee_count=sizeof(kTz__TZE204_lbbg34rj)/sizeof(kTz__TZE204_lbbg34rj[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
