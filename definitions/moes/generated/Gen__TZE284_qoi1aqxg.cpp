// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_qoi1aqxg).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_qoi1aqxg_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_qoi1aqxg_motor_direction_dp11[] = {
    { 0, "forward" },
    { 1, "back" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_qoi1aqxg_border_dp16[] = {
    { 0, "up" },
    { 1, "down" },
    { 2, "up_delete" },
    { 3, "down_delete" },
    { 4, "remove_top_bottom" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_qoi1aqxg[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_qoi1aqxg_state_dp1, sizeof(kEnum__TZE284_qoi1aqxg_state_dp1)/sizeof(kEnum__TZE284_qoi1aqxg_state_dp1[0]) },
    { 9, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 11, "motor_direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_qoi1aqxg_motor_direction_dp11, sizeof(kEnum__TZE284_qoi1aqxg_motor_direction_dp11)/sizeof(kEnum__TZE284_qoi1aqxg_motor_direction_dp11[0]) },
    { 16, "border", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_qoi1aqxg_border_dp16, sizeof(kEnum__TZE284_qoi1aqxg_border_dp16)/sizeof(kEnum__TZE284_qoi1aqxg_border_dp16[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_qoi1aqxg{ kEntries__TZE284_qoi1aqxg, 5 };
constexpr FzConverter kFzDp__TZE284_qoi1aqxg{
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
    .user_config       = &kMap__TZE284_qoi1aqxg,
};
const FzConverter* const kFz__TZE284_qoi1aqxg[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_qoi1aqxg,
};
constexpr TzConverter kTzDp__TZE284_qoi1aqxg{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_qoi1aqxg,
};
const TzConverter* const kTz__TZE284_qoi1aqxg[] = { &kTzDp__TZE284_qoi1aqxg };
constexpr const char* kM__TZE284_qoi1aqxg[] = { "TS0601" };
constexpr const char* kN__TZE284_qoi1aqxg[] = { "_TZE284_qoi1aqxg" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_qoi1aqxg{
    .zigbee_models=kM__TZE284_qoi1aqxg,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_qoi1aqxg,.manufacturer_names_count=1,
    .model="TS0601__TZE284_qoi1aqxg",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_qoi1aqxg,
    .from_zigbee_count=sizeof(kFz__TZE284_qoi1aqxg)/sizeof(kFz__TZE284_qoi1aqxg[0]),
    .to_zigbee=kTz__TZE284_qoi1aqxg,
    .to_zigbee_count=sizeof(kTz__TZE284_qoi1aqxg)/sizeof(kTz__TZE284_qoi1aqxg[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
