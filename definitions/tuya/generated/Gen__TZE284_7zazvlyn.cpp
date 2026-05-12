// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_7zazvlyn).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_7zazvlyn_cover_state_l1_dp133[] = {
    { 0, "open" },
    { 1, "stop" },
    { 2, "close" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_7zazvlyn_cover_state_l2_dp134[] = {
    { 0, "open" },
    { 1, "stop" },
    { 2, "close" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_7zazvlyn_cover_state_l3_dp135[] = {
    { 0, "open" },
    { 1, "stop" },
    { 2, "close" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_7zazvlyn_cover_state_l4_dp136[] = {
    { 0, "open" },
    { 1, "stop" },
    { 2, "close" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_7zazvlyn[] = {
    { 1, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 2, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 6, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "led_bright_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "led_bright_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "led_bright_l3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "led_bright_l4", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "led_warm_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 110, "led_warm_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "led_warm_l3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 112, "led_warm_l4", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 113, "cover_position_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 114, "cover_position_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 115, "cover_position_l3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 116, "cover_position_l4", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 117, "led_switch_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 118, "led_switch_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 119, "led_switch_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 120, "led_switch_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 121, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 122, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 123, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 124, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 125, "led_switch_name_l1", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 126, "led_switch_name_l2", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 127, "led_switch_name_l3", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 128, "led_switch_name_l4", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 129, "cover_name_l1", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 130, "cover_name_l2", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 131, "cover_name_l3", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 132, "cover_name_l4", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 133, "cover_state_l1", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_7zazvlyn_cover_state_l1_dp133, sizeof(kEnum__TZE284_7zazvlyn_cover_state_l1_dp133)/sizeof(kEnum__TZE284_7zazvlyn_cover_state_l1_dp133[0]) },
    { 134, "cover_state_l2", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_7zazvlyn_cover_state_l2_dp134, sizeof(kEnum__TZE284_7zazvlyn_cover_state_l2_dp134)/sizeof(kEnum__TZE284_7zazvlyn_cover_state_l2_dp134[0]) },
    { 135, "cover_state_l3", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_7zazvlyn_cover_state_l3_dp135, sizeof(kEnum__TZE284_7zazvlyn_cover_state_l3_dp135)/sizeof(kEnum__TZE284_7zazvlyn_cover_state_l3_dp135[0]) },
    { 136, "cover_state_l4", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_7zazvlyn_cover_state_l4_dp136, sizeof(kEnum__TZE284_7zazvlyn_cover_state_l4_dp136)/sizeof(kEnum__TZE284_7zazvlyn_cover_state_l4_dp136[0]) },
    { 137, "switch_name_l1", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 138, "switch_name_l2", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 139, "switch_name_l3", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 140, "switch_name_l4", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 141, "scene_name_l1", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 142, "scene_name_l2", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 143, "scene_name_l3", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 144, "scene_name_l4", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 145, "scene_name_l5", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 146, "scene_name_l6", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 147, "scene_name_l7", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 148, "scene_name_l8", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 149, "backlight_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_7zazvlyn{ kEntries__TZE284_7zazvlyn, 53 };
constexpr FzConverter kFzDp__TZE284_7zazvlyn{
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
    .user_config       = &kMap__TZE284_7zazvlyn,
};
const FzConverter* const kFz__TZE284_7zazvlyn[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_7zazvlyn,
};
constexpr TzConverter kTzDp__TZE284_7zazvlyn{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_7zazvlyn,
};
const TzConverter* const kTz__TZE284_7zazvlyn[] = { &kTzDp__TZE284_7zazvlyn };
constexpr const char* kM__TZE284_7zazvlyn[] = { "TS0601" };
constexpr const char* kN__TZE284_7zazvlyn[] = { "_TZE284_7zazvlyn" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_7zazvlyn{
    .zigbee_models=kM__TZE284_7zazvlyn,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_7zazvlyn,.manufacturer_names_count=1,
    .model="TS0601__TZE284_7zazvlyn",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_7zazvlyn,
    .from_zigbee_count=sizeof(kFz__TZE284_7zazvlyn)/sizeof(kFz__TZE284_7zazvlyn[0]),
    .to_zigbee=kTz__TZE284_7zazvlyn,
    .to_zigbee_count=sizeof(kTz__TZE284_7zazvlyn)/sizeof(kTz__TZE284_7zazvlyn[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
