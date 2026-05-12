// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_zqq3cipq).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_zqq3cipq_power_on_behavior_dp117[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_zqq3cipq_switch_type_l1_l8_dp118[] = {
    { 0, "momentary" },
    { 1, "toggle" },
    { 2, "state" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_zqq3cipq_switch_type_l9_l16_dp119[] = {
    { 0, "momentary" },
    { 1, "toggle" },
    { 2, "state" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_zqq3cipq_switch_mode_l11_l12_dp120[] = {
    { 0, "switch" },
    { 1, "scene" },
    { 2, "light" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_zqq3cipq_switch_mode_l13_l14_dp121[] = {
    { 0, "switch" },
    { 1, "scene" },
    { 2, "light" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_zqq3cipq_switch_mode_l15_l16_dp122[] = {
    { 0, "switch" },
    { 1, "scene" },
    { 2, "light" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_zqq3cipq[] = {
    { 1, "state_all", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 101, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 103, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 104, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 105, "state_l5", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 106, "state_l6", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 107, "state_l7", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 108, "state_l8", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 109, "state_l9", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 110, "state_l10", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 111, "state_l11", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 112, "state_l12", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 113, "state_l13", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 114, "state_l14", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 115, "state_l15", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 116, "state_l16", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 117, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_zqq3cipq_power_on_behavior_dp117, sizeof(kEnum__TZE284_zqq3cipq_power_on_behavior_dp117)/sizeof(kEnum__TZE284_zqq3cipq_power_on_behavior_dp117[0]) },
    { 118, "switch_type_l1_l8", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_zqq3cipq_switch_type_l1_l8_dp118, sizeof(kEnum__TZE284_zqq3cipq_switch_type_l1_l8_dp118)/sizeof(kEnum__TZE284_zqq3cipq_switch_type_l1_l8_dp118[0]) },
    { 119, "switch_type_l9_l16", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_zqq3cipq_switch_type_l9_l16_dp119, sizeof(kEnum__TZE284_zqq3cipq_switch_type_l9_l16_dp119)/sizeof(kEnum__TZE284_zqq3cipq_switch_type_l9_l16_dp119[0]) },
    { 120, "switch_mode_l11_l12", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_zqq3cipq_switch_mode_l11_l12_dp120, sizeof(kEnum__TZE284_zqq3cipq_switch_mode_l11_l12_dp120)/sizeof(kEnum__TZE284_zqq3cipq_switch_mode_l11_l12_dp120[0]) },
    { 121, "switch_mode_l13_l14", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_zqq3cipq_switch_mode_l13_l14_dp121, sizeof(kEnum__TZE284_zqq3cipq_switch_mode_l13_l14_dp121)/sizeof(kEnum__TZE284_zqq3cipq_switch_mode_l13_l14_dp121[0]) },
    { 122, "switch_mode_l15_l16", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_zqq3cipq_switch_mode_l15_l16_dp122, sizeof(kEnum__TZE284_zqq3cipq_switch_mode_l15_l16_dp122)/sizeof(kEnum__TZE284_zqq3cipq_switch_mode_l15_l16_dp122[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_zqq3cipq{ kEntries__TZE284_zqq3cipq, 23 };
constexpr FzConverter kFzDp__TZE284_zqq3cipq{
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
    .user_config       = &kMap__TZE284_zqq3cipq,
};
const FzConverter* const kFz__TZE284_zqq3cipq[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_zqq3cipq,
};
constexpr TzConverter kTzDp__TZE284_zqq3cipq{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_zqq3cipq,
};
const TzConverter* const kTz__TZE284_zqq3cipq[] = { &kTzDp__TZE284_zqq3cipq };
constexpr const char* kM__TZE284_zqq3cipq[] = { "TS0601" };
constexpr const char* kN__TZE284_zqq3cipq[] = { "_TZE284_zqq3cipq" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_zqq3cipq{
    .zigbee_models=kM__TZE284_zqq3cipq,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_zqq3cipq,.manufacturer_names_count=1,
    .model="TS0601__TZE284_zqq3cipq",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_zqq3cipq,
    .from_zigbee_count=sizeof(kFz__TZE284_zqq3cipq)/sizeof(kFz__TZE284_zqq3cipq[0]),
    .to_zigbee=kTz__TZE284_zqq3cipq,
    .to_zigbee_count=sizeof(kTz__TZE284_zqq3cipq)/sizeof(kTz__TZE284_zqq3cipq[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
