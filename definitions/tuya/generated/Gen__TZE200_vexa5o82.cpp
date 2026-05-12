// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_vexa5o82).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_vexa5o82_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_vexa5o82_reverse_direction_dp5[] = {
    { 0, "forward" },
    { 1, "back" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_vexa5o82_border_dp16[] = {
    { 0, "up" },
    { 1, "down" },
    { 2, "up_delete" },
    { 3, "down_delete" },
    { 4, "remove_top_bottom" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_vexa5o82_click_control_dp20[] = {
    { 0, "up" },
    { 1, "down" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_vexa5o82[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_vexa5o82_state_dp1, sizeof(kEnum__TZE200_vexa5o82_state_dp1)/sizeof(kEnum__TZE200_vexa5o82_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "reverse_direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_vexa5o82_reverse_direction_dp5, sizeof(kEnum__TZE200_vexa5o82_reverse_direction_dp5)/sizeof(kEnum__TZE200_vexa5o82_reverse_direction_dp5[0]) },
    { 12, "motor_fault", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 13, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "border", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_vexa5o82_border_dp16, sizeof(kEnum__TZE200_vexa5o82_border_dp16)/sizeof(kEnum__TZE200_vexa5o82_border_dp16[0]) },
    { 20, "click_control", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_vexa5o82_click_control_dp20, sizeof(kEnum__TZE200_vexa5o82_click_control_dp20)/sizeof(kEnum__TZE200_vexa5o82_click_control_dp20[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_vexa5o82{ kEntries__TZE200_vexa5o82, 8 };
constexpr FzConverter kFzDp__TZE200_vexa5o82{
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
    .user_config       = &kMap__TZE200_vexa5o82,
};
const FzConverter* const kFz__TZE200_vexa5o82[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_vexa5o82,
};
constexpr TzConverter kTzDp__TZE200_vexa5o82{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_vexa5o82,
};
const TzConverter* const kTz__TZE200_vexa5o82[] = { &kTzDp__TZE200_vexa5o82 };
constexpr const char* kM__TZE200_vexa5o82[] = { "TS0601" };
constexpr const char* kN__TZE200_vexa5o82[] = { "_TZE200_vexa5o82" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_vexa5o82{
    .zigbee_models=kM__TZE200_vexa5o82,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_vexa5o82,.manufacturer_names_count=1,
    .model="TS0601__TZE200_vexa5o82",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_vexa5o82,
    .from_zigbee_count=sizeof(kFz__TZE200_vexa5o82)/sizeof(kFz__TZE200_vexa5o82[0]),
    .to_zigbee=kTz__TZE200_vexa5o82,
    .to_zigbee_count=sizeof(kTz__TZE200_vexa5o82)/sizeof(kTz__TZE200_vexa5o82[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
