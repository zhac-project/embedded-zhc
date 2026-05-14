// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_stvgmdjz).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_stvgmdjz_mode_l1_dp18[] = {
    { 0, "switch_1" },
    { 1, "scene_1" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_stvgmdjz_indicator_status_dp37[] = {
    { 0, "off" },
    { 1, "relay" },
    { 2, "invert" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_stvgmdjz_power_on_behavior_dp38[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_stvgmdjz[] = {
    { 1, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "mode_l1", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_stvgmdjz_mode_l1_dp18, sizeof(kEnum__TZE200_stvgmdjz_mode_l1_dp18)/sizeof(kEnum__TZE200_stvgmdjz_mode_l1_dp18[0]) },
    { 24, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 30, "countdown_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 36, "backlight_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 37, "indicator_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_stvgmdjz_indicator_status_dp37, sizeof(kEnum__TZE200_stvgmdjz_indicator_status_dp37)/sizeof(kEnum__TZE200_stvgmdjz_indicator_status_dp37[0]) },
    { 38, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_stvgmdjz_power_on_behavior_dp38, sizeof(kEnum__TZE200_stvgmdjz_power_on_behavior_dp38)/sizeof(kEnum__TZE200_stvgmdjz_power_on_behavior_dp38[0]) },
    { 103, "induction_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 105, "momentary_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_stvgmdjz{ kEntries__TZE200_stvgmdjz, 9 };
constexpr FzConverter kFzDp__TZE200_stvgmdjz{
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
    .user_config       = &kMap__TZE200_stvgmdjz,
};
const FzConverter* const kFz__TZE200_stvgmdjz[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_stvgmdjz,
};
constexpr TzConverter kTzDp__TZE200_stvgmdjz{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_stvgmdjz,
};
const TzConverter* const kTz__TZE200_stvgmdjz[] = { &kTzDp__TZE200_stvgmdjz };
constexpr const char* kM__TZE200_stvgmdjz[] = { "TS0601" };
constexpr const char* kN__TZE200_stvgmdjz[] = { "_TZE200_stvgmdjz" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_stvgmdjz{
    .zigbee_models=kM__TZE200_stvgmdjz,.zigbee_models_count=sizeof(kM__TZE200_stvgmdjz)/sizeof(kM__TZE200_stvgmdjz[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_stvgmdjz,.manufacturer_names_count=1,
    .model="TS0601__TZE200_stvgmdjz",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_stvgmdjz,
    .from_zigbee_count=sizeof(kFz__TZE200_stvgmdjz)/sizeof(kFz__TZE200_stvgmdjz[0]),
    .to_zigbee=kTz__TZE200_stvgmdjz,
    .to_zigbee_count=sizeof(kTz__TZE200_stvgmdjz)/sizeof(kTz__TZE200_stvgmdjz[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
