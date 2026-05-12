// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_1drr8tab).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_1drr8tab_system_mode_dp2[] = {
    { 1, "heat" },
    { 0, "cool" },
    { 2, "fan_only" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_1drr8tab_fan_mode_dp28[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
    { 3, "auto" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_1drr8tab[] = {
    { 1, "air_switch", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 2, "system_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_1drr8tab_system_mode_dp2, sizeof(kEnum__TZE200_1drr8tab_system_mode_dp2)/sizeof(kEnum__TZE200_1drr8tab_system_mode_dp2[0]) },
    { 28, "fan_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_1drr8tab_fan_mode_dp28, sizeof(kEnum__TZE200_1drr8tab_fan_mode_dp28)/sizeof(kEnum__TZE200_1drr8tab_fan_mode_dp28[0]) },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 40, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 101, "boost_time", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_1drr8tab{ kEntries__TZE200_1drr8tab, 7 };
constexpr FzConverter kFzDp__TZE200_1drr8tab{
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
    .user_config       = &kMap__TZE200_1drr8tab,
};
const FzConverter* const kFz__TZE200_1drr8tab[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_1drr8tab,
};
constexpr TzConverter kTzDp__TZE200_1drr8tab{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_1drr8tab,
};
const TzConverter* const kTz__TZE200_1drr8tab[] = { &kTzDp__TZE200_1drr8tab };
constexpr const char* kM__TZE200_1drr8tab[] = { "TS0601" };
constexpr const char* kN__TZE200_1drr8tab[] = { "_TZE200_1drr8tab" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_1drr8tab{
    .zigbee_models=kM__TZE200_1drr8tab,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_1drr8tab,.manufacturer_names_count=1,
    .model="TS0601__TZE200_1drr8tab",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_1drr8tab,
    .from_zigbee_count=sizeof(kFz__TZE200_1drr8tab)/sizeof(kFz__TZE200_1drr8tab[0]),
    .to_zigbee=kTz__TZE200_1drr8tab,
    .to_zigbee_count=sizeof(kTz__TZE200_1drr8tab)/sizeof(kTz__TZE200_1drr8tab[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
