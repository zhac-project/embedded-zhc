// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_ex3rcdha).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_ex3rcdha[] = {
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
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_ex3rcdha{ kEntries__TZE204_ex3rcdha, 9 };
constexpr FzConverter kFzDp__TZE204_ex3rcdha{
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
    .user_config       = &kMap__TZE204_ex3rcdha,
};
const FzConverter* const kFz__TZE204_ex3rcdha[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_ex3rcdha,
};
constexpr TzConverter kTzDp__TZE204_ex3rcdha{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_ex3rcdha,
};
const TzConverter* const kTz__TZE204_ex3rcdha[] = { &kTzDp__TZE204_ex3rcdha };
constexpr const char* kM__TZE204_ex3rcdha[] = { "TS0601" };
constexpr const char* kN__TZE204_ex3rcdha[] = { "_TZE204_ex3rcdha" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_ex3rcdha[] = {
    {"Nova Digital","ZTS-MM"},
};
extern const PreparedDefinition kDefGen__TZE204_ex3rcdha{
    .zigbee_models=kM__TZE204_ex3rcdha,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_ex3rcdha,.manufacturer_names_count=1,
    .model="TS0601__TZE204_ex3rcdha",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_ex3rcdha, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_ex3rcdha)/sizeof(kWhiteLabels_Gen_TZE204_ex3rcdha[0]),
    .from_zigbee=kFz__TZE204_ex3rcdha,
    .from_zigbee_count=sizeof(kFz__TZE204_ex3rcdha)/sizeof(kFz__TZE204_ex3rcdha[0]),
    .to_zigbee=kTz__TZE204_ex3rcdha,
    .to_zigbee_count=sizeof(kTz__TZE204_ex3rcdha)/sizeof(kTz__TZE204_ex3rcdha[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
