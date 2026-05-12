// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_wunufsil).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_wunufsil_power_on_behavior_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_wunufsil[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 7, "countdown_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "countdown_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 13, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 14, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_wunufsil_power_on_behavior_dp14, sizeof(kEnum__TZE200_wunufsil_power_on_behavior_dp14)/sizeof(kEnum__TZE200_wunufsil_power_on_behavior_dp14[0]) },
    { 16, "backlight_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_wunufsil{ kEntries__TZE200_wunufsil, 7 };
constexpr FzConverter kFzDp__TZE200_wunufsil{
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
    .user_config       = &kMap__TZE200_wunufsil,
};
const FzConverter* const kFz__TZE200_wunufsil[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_wunufsil,
};
constexpr TzConverter kTzDp__TZE200_wunufsil{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_wunufsil,
};
const TzConverter* const kTz__TZE200_wunufsil[] = { &kTzDp__TZE200_wunufsil };
constexpr const char* kM__TZE200_wunufsil[] = { "TS0601" };
constexpr const char* kN__TZE200_wunufsil[] = { "_TZE200_wunufsil" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_wunufsil{
    .zigbee_models=kM__TZE200_wunufsil,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_wunufsil,.manufacturer_names_count=1,
    .model="TS0601__TZE200_wunufsil",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_wunufsil,
    .from_zigbee_count=sizeof(kFz__TZE200_wunufsil)/sizeof(kFz__TZE200_wunufsil[0]),
    .to_zigbee=kTz__TZE200_wunufsil,
    .to_zigbee_count=sizeof(kTz__TZE200_wunufsil)/sizeof(kTz__TZE200_wunufsil[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
