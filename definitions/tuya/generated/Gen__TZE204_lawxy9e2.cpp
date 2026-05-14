// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_lawxy9e2).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_lawxy9e2_fan_speed_dp3[] = {
    { 0, "1" },
    { 1, "2" },
    { 2, "3" },
    { 3, "4" },
    { 4, "5" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_lawxy9e2_power_on_behavior_dp11[] = {
    { 0, "OFF" },
    { 1, "ON" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_lawxy9e2[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "fan_speed", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_lawxy9e2_fan_speed_dp3, sizeof(kEnum__TZE204_lawxy9e2_fan_speed_dp3)/sizeof(kEnum__TZE204_lawxy9e2_fan_speed_dp3[0]) },
    { 11, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_lawxy9e2_power_on_behavior_dp11, sizeof(kEnum__TZE204_lawxy9e2_power_on_behavior_dp11)/sizeof(kEnum__TZE204_lawxy9e2_power_on_behavior_dp11[0]) },
    { 5, "status_indication", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_lawxy9e2{ kEntries__TZE204_lawxy9e2, 4 };
constexpr FzConverter kFzDp__TZE204_lawxy9e2{
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
    .user_config       = &kMap__TZE204_lawxy9e2,
};
const FzConverter* const kFz__TZE204_lawxy9e2[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_lawxy9e2,
};
constexpr TzConverter kTzDp__TZE204_lawxy9e2{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_lawxy9e2,
};
const TzConverter* const kTz__TZE204_lawxy9e2[] = { &kTzDp__TZE204_lawxy9e2 };
constexpr const char* kM__TZE204_lawxy9e2[] = { "TS0601" };
constexpr const char* kN__TZE204_lawxy9e2[] = { "_TZE204_lawxy9e2" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_lawxy9e2[] = {
    {"Liwokit","Fan+Light-01"},
};
extern const PreparedDefinition kDefGen__TZE204_lawxy9e2{
    .zigbee_models=kM__TZE204_lawxy9e2,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_lawxy9e2,.manufacturer_names_count=1,
    .model="TS0601__TZE204_lawxy9e2",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_lawxy9e2, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_lawxy9e2)/sizeof(kWhiteLabels_Gen_TZE204_lawxy9e2[0]),
    .from_zigbee=kFz__TZE204_lawxy9e2,
    .from_zigbee_count=sizeof(kFz__TZE204_lawxy9e2)/sizeof(kFz__TZE204_lawxy9e2[0]),
    .to_zigbee=kTz__TZE204_lawxy9e2,
    .to_zigbee_count=sizeof(kTz__TZE204_lawxy9e2)/sizeof(kTz__TZE204_lawxy9e2[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
