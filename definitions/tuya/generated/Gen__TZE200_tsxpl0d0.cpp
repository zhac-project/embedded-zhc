// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_tsxpl0d0).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_tsxpl0d0_light_type_l1_dp4[] = {
    { 0, "led" },
    { 1, "incandescent" },
    { 2, "halogen" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_tsxpl0d0_light_type_l2_dp10[] = {
    { 0, "led" },
    { 1, "incandescent" },
    { 2, "halogen" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_tsxpl0d0_power_on_behavior_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_tsxpl0d0[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "brightness_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "min_brightness_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "light_type_l1", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_tsxpl0d0_light_type_l1_dp4, sizeof(kEnum__TZE200_tsxpl0d0_light_type_l1_dp4)/sizeof(kEnum__TZE200_tsxpl0d0_light_type_l1_dp4[0]) },
    { 5, "max_brightness_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 6, "countdown_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 8, "brightness_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "min_brightness_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 10, "light_type_l2", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_tsxpl0d0_light_type_l2_dp10, sizeof(kEnum__TZE200_tsxpl0d0_light_type_l2_dp10)/sizeof(kEnum__TZE200_tsxpl0d0_light_type_l2_dp10[0]) },
    { 11, "max_brightness_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 12, "countdown_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_tsxpl0d0_power_on_behavior_dp14, sizeof(kEnum__TZE200_tsxpl0d0_power_on_behavior_dp14)/sizeof(kEnum__TZE200_tsxpl0d0_power_on_behavior_dp14[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_tsxpl0d0{ kEntries__TZE200_tsxpl0d0, 13 };
constexpr FzConverter kFzDp__TZE200_tsxpl0d0{
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
    .user_config       = &kMap__TZE200_tsxpl0d0,
};
const FzConverter* const kFz__TZE200_tsxpl0d0[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_tsxpl0d0,
};
constexpr TzConverter kTzDp__TZE200_tsxpl0d0{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_tsxpl0d0,
};
const TzConverter* const kTz__TZE200_tsxpl0d0[] = { &kTzDp__TZE200_tsxpl0d0 };
constexpr const char* kM__TZE200_tsxpl0d0[] = { "TS0601" };
constexpr const char* kN__TZE200_tsxpl0d0[] = { "_TZE200_tsxpl0d0" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_tsxpl0d0[] = {
    {"Moes","ZM-105B-M"},
    {"KnockautX","FMD2C018"},
};
extern const PreparedDefinition kDefGen__TZE200_tsxpl0d0{
    .zigbee_models=kM__TZE200_tsxpl0d0,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_tsxpl0d0,.manufacturer_names_count=1,
    .model="TS0601__TZE200_tsxpl0d0",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_tsxpl0d0, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_tsxpl0d0)/sizeof(kWhiteLabels_Gen_TZE200_tsxpl0d0[0]),
    .from_zigbee=kFz__TZE200_tsxpl0d0,
    .from_zigbee_count=sizeof(kFz__TZE200_tsxpl0d0)/sizeof(kFz__TZE200_tsxpl0d0[0]),
    .to_zigbee=kTz__TZE200_tsxpl0d0,
    .to_zigbee_count=sizeof(kTz__TZE200_tsxpl0d0)/sizeof(kTz__TZE200_tsxpl0d0[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
