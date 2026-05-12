// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_a1ovdobn).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_a1ovdobn_relay_status_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "memory" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_a1ovdobn_light_mode_dp21[] = {
    { 0, "none" },
    { 1, "relay" },
    { 2, "pos" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_a1ovdobn[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "brightness_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "brightness_min_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "brightness_max_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 6, "countdown_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "relay_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_a1ovdobn_relay_status_dp14, sizeof(kEnum__TZE284_a1ovdobn_relay_status_dp14)/sizeof(kEnum__TZE284_a1ovdobn_relay_status_dp14[0]) },
    { 21, "light_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_a1ovdobn_light_mode_dp21, sizeof(kEnum__TZE284_a1ovdobn_light_mode_dp21)/sizeof(kEnum__TZE284_a1ovdobn_light_mode_dp21[0]) },
    { 26, "switch_backlight", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_a1ovdobn{ kEntries__TZE284_a1ovdobn, 8 };
constexpr FzConverter kFzDp__TZE284_a1ovdobn{
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
    .user_config       = &kMap__TZE284_a1ovdobn,
};
const FzConverter* const kFz__TZE284_a1ovdobn[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_a1ovdobn,
};
constexpr TzConverter kTzDp__TZE284_a1ovdobn{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_a1ovdobn,
};
const TzConverter* const kTz__TZE284_a1ovdobn[] = { &kTzDp__TZE284_a1ovdobn };
constexpr const char* kM__TZE284_a1ovdobn[] = { "TS0601" };
constexpr const char* kN__TZE284_a1ovdobn[] = { "_TZE284_a1ovdobn" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_a1ovdobn{
    .zigbee_models=kM__TZE284_a1ovdobn,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_a1ovdobn,.manufacturer_names_count=1,
    .model="TS0601__TZE284_a1ovdobn",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_a1ovdobn,
    .from_zigbee_count=sizeof(kFz__TZE284_a1ovdobn)/sizeof(kFz__TZE284_a1ovdobn[0]),
    .to_zigbee=kTz__TZE284_a1ovdobn,
    .to_zigbee_count=sizeof(kTz__TZE284_a1ovdobn)/sizeof(kTz__TZE284_a1ovdobn[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
