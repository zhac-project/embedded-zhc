// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_rlqamjhp).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_rlqamjhp_relay_status_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "memory" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_rlqamjhp_light_mode_dp21[] = {
    { 0, "none" },
    { 1, "relay" },
    { 2, "pos" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_rlqamjhp[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "brightness_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "brightness_min_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "brightness_max_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 6, "countdown_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 8, "brightness_2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "brightness_min_2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 11, "brightness_max_2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 12, "countdown_2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "relay_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_rlqamjhp_relay_status_dp14, sizeof(kEnum__TZE200_rlqamjhp_relay_status_dp14)/sizeof(kEnum__TZE200_rlqamjhp_relay_status_dp14[0]) },
    { 21, "light_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_rlqamjhp_light_mode_dp21, sizeof(kEnum__TZE200_rlqamjhp_light_mode_dp21)/sizeof(kEnum__TZE200_rlqamjhp_light_mode_dp21[0]) },
    { 26, "switch_backlight", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_rlqamjhp{ kEntries__TZE200_rlqamjhp, 13 };
constexpr FzConverter kFzDp__TZE200_rlqamjhp{
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
    .user_config       = &kMap__TZE200_rlqamjhp,
};
const FzConverter* const kFz__TZE200_rlqamjhp[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_rlqamjhp,
};
constexpr TzConverter kTzDp__TZE200_rlqamjhp{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_rlqamjhp,
};
const TzConverter* const kTz__TZE200_rlqamjhp[] = { &kTzDp__TZE200_rlqamjhp };
constexpr const char* kM__TZE200_rlqamjhp[] = { "TS0601" };
constexpr const char* kN__TZE200_rlqamjhp[] = { "_TZE200_rlqamjhp" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_rlqamjhp{
    .zigbee_models=kM__TZE200_rlqamjhp,.zigbee_models_count=sizeof(kM__TZE200_rlqamjhp)/sizeof(kM__TZE200_rlqamjhp[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_rlqamjhp,.manufacturer_names_count=1,
    .model="TS0601__TZE200_rlqamjhp",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_rlqamjhp,
    .from_zigbee_count=sizeof(kFz__TZE200_rlqamjhp)/sizeof(kFz__TZE200_rlqamjhp[0]),
    .to_zigbee=kTz__TZE200_rlqamjhp,
    .to_zigbee_count=sizeof(kTz__TZE200_rlqamjhp)/sizeof(kTz__TZE200_rlqamjhp[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
