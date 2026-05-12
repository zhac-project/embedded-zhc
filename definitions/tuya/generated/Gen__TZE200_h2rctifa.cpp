// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_h2rctifa).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_h2rctifa_power_on_behavior_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_h2rctifa[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 5, "state_l5", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 6, "state_l6", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 7, "countdown_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "countdown_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "countdown_l3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 10, "countdown_l4", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 11, "countdown_l5", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 12, "countdown_l6", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 13, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 14, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_h2rctifa_power_on_behavior_dp14, sizeof(kEnum__TZE200_h2rctifa_power_on_behavior_dp14)/sizeof(kEnum__TZE200_h2rctifa_power_on_behavior_dp14[0]) },
    { 16, "backlight_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_h2rctifa{ kEntries__TZE200_h2rctifa, 15 };
constexpr FzConverter kFzDp__TZE200_h2rctifa{
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
    .user_config       = &kMap__TZE200_h2rctifa,
};
const FzConverter* const kFz__TZE200_h2rctifa[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_h2rctifa,
};
constexpr TzConverter kTzDp__TZE200_h2rctifa{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_h2rctifa,
};
const TzConverter* const kTz__TZE200_h2rctifa[] = { &kTzDp__TZE200_h2rctifa };
constexpr const char* kM__TZE200_h2rctifa[] = { "TS0601" };
constexpr const char* kN__TZE200_h2rctifa[] = { "_TZE200_h2rctifa" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_h2rctifa{
    .zigbee_models=kM__TZE200_h2rctifa,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_h2rctifa,.manufacturer_names_count=1,
    .model="TS0601__TZE200_h2rctifa",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_h2rctifa,
    .from_zigbee_count=sizeof(kFz__TZE200_h2rctifa)/sizeof(kFz__TZE200_h2rctifa[0]),
    .to_zigbee=kTz__TZE200_h2rctifa,
    .to_zigbee_count=sizeof(kTz__TZE200_h2rctifa)/sizeof(kTz__TZE200_h2rctifa[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
