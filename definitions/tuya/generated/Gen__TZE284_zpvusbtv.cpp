// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_zpvusbtv).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_zpvusbtv_power_on_behavior_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_zpvusbtv_indicator_mode_dp15[] = {
    { 0, "off" },
    { 1, "on_off_status" },
    { 2, "switch_position" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_zpvusbtv_on_color_dp103[] = {
    { 0, "red" },
    { 1, "blue" },
    { 2, "green" },
    { 3, "white" },
    { 4, "yellow" },
    { 5, "magenta" },
    { 6, "cyan" },
    { 7, "warm_white" },
    { 8, "warm_yellow" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_zpvusbtv_off_color_dp104[] = {
    { 0, "red" },
    { 1, "blue" },
    { 2, "green" },
    { 3, "white" },
    { 4, "yellow" },
    { 5, "magenta" },
    { 6, "cyan" },
    { 7, "warm_white" },
    { 8, "warm_yellow" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_zpvusbtv[] = {
    { 13, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 7, "countdown_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "countdown_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_zpvusbtv_power_on_behavior_dp14, sizeof(kEnum__TZE284_zpvusbtv_power_on_behavior_dp14)/sizeof(kEnum__TZE284_zpvusbtv_power_on_behavior_dp14[0]) },
    { 15, "indicator_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_zpvusbtv_indicator_mode_dp15, sizeof(kEnum__TZE284_zpvusbtv_indicator_mode_dp15)/sizeof(kEnum__TZE284_zpvusbtv_indicator_mode_dp15[0]) },
    { 16, "backlight_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 101, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "backlight", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "on_color", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_zpvusbtv_on_color_dp103, sizeof(kEnum__TZE284_zpvusbtv_on_color_dp103)/sizeof(kEnum__TZE284_zpvusbtv_on_color_dp103[0]) },
    { 104, "off_color", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_zpvusbtv_off_color_dp104, sizeof(kEnum__TZE284_zpvusbtv_off_color_dp104)/sizeof(kEnum__TZE284_zpvusbtv_off_color_dp104[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_zpvusbtv{ kEntries__TZE284_zpvusbtv, 12 };
constexpr FzConverter kFzDp__TZE284_zpvusbtv{
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
    .user_config       = &kMap__TZE284_zpvusbtv,
};
const FzConverter* const kFz__TZE284_zpvusbtv[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_zpvusbtv,
};
constexpr TzConverter kTzDp__TZE284_zpvusbtv{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_zpvusbtv,
};
const TzConverter* const kTz__TZE284_zpvusbtv[] = { &kTzDp__TZE284_zpvusbtv };
constexpr const char* kM__TZE284_zpvusbtv[] = { "TS0601" };
constexpr const char* kN__TZE284_zpvusbtv[] = { "_TZE284_zpvusbtv" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_zpvusbtv{
    .zigbee_models=kM__TZE284_zpvusbtv,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_zpvusbtv,.manufacturer_names_count=1,
    .model="TS0601__TZE284_zpvusbtv",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_zpvusbtv,
    .from_zigbee_count=sizeof(kFz__TZE284_zpvusbtv)/sizeof(kFz__TZE284_zpvusbtv[0]),
    .to_zigbee=kTz__TZE284_zpvusbtv,
    .to_zigbee_count=sizeof(kTz__TZE284_zpvusbtv)/sizeof(kTz__TZE284_zpvusbtv[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
