// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_r32ctezx).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_r32ctezx_fan_speed_dp3[] = {
    { 0, "1" },
    { 1, "2" },
    { 2, "3" },
    { 3, "4" },
    { 4, "5" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_r32ctezx_power_on_behavior_dp11[] = {
    { 0, "off" },
    { 1, "on" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_r32ctezx[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "fan_speed", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_r32ctezx_fan_speed_dp3, sizeof(kEnum__TZE200_r32ctezx_fan_speed_dp3)/sizeof(kEnum__TZE200_r32ctezx_fan_speed_dp3[0]) },
    { 11, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_r32ctezx_power_on_behavior_dp11, sizeof(kEnum__TZE200_r32ctezx_power_on_behavior_dp11)/sizeof(kEnum__TZE200_r32ctezx_power_on_behavior_dp11[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_r32ctezx{ kEntries__TZE200_r32ctezx, 4 };
constexpr FzConverter kFzDp__TZE200_r32ctezx{
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
    .user_config       = &kMap__TZE200_r32ctezx,
};
const FzConverter* const kFz__TZE200_r32ctezx[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_r32ctezx,
};
constexpr TzConverter kTzDp__TZE200_r32ctezx{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_r32ctezx,
};
const TzConverter* const kTz__TZE200_r32ctezx[] = { &kTzDp__TZE200_r32ctezx };
constexpr const char* kM__TZE200_r32ctezx[] = { "TS0601" };
constexpr const char* kN__TZE200_r32ctezx[] = { "_TZE200_r32ctezx" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_r32ctezx[] = {
    {"Lerlink","T2-Z67/T2-W67"},
};
extern const PreparedDefinition kDefGen__TZE200_r32ctezx{
    .zigbee_models=kM__TZE200_r32ctezx,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_r32ctezx,.manufacturer_names_count=1,
    .model="TS0601__TZE200_r32ctezx",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_r32ctezx, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_r32ctezx)/sizeof(kWhiteLabels_Gen_TZE200_r32ctezx[0]),
    .from_zigbee=kFz__TZE200_r32ctezx,
    .from_zigbee_count=sizeof(kFz__TZE200_r32ctezx)/sizeof(kFz__TZE200_r32ctezx[0]),
    .to_zigbee=kTz__TZE200_r32ctezx,
    .to_zigbee_count=sizeof(kTz__TZE200_r32ctezx)/sizeof(kTz__TZE200_r32ctezx[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
