// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0021 / _TZ3210_3ulg9kpo).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZ3210_3ulg9kpo_action_dp1[] = {
    { 0, "button_1_single" },
    { 1, "button_1_double" },
    { 2, "button_1_hold" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZ3210_3ulg9kpo_action_dp2[] = {
    { 0, "button_2_single" },
    { 1, "button_2_double" },
    { 2, "button_2_hold" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZ3210_3ulg9kpo[] = {
    { 1, "action", ::zhc::TuyaDpType::Enum, 1, kEnum__TZ3210_3ulg9kpo_action_dp1, sizeof(kEnum__TZ3210_3ulg9kpo_action_dp1)/sizeof(kEnum__TZ3210_3ulg9kpo_action_dp1[0]) },
    { 2, "action", ::zhc::TuyaDpType::Enum, 1, kEnum__TZ3210_3ulg9kpo_action_dp2, sizeof(kEnum__TZ3210_3ulg9kpo_action_dp2)/sizeof(kEnum__TZ3210_3ulg9kpo_action_dp2[0]) },
    { 10, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZ3210_3ulg9kpo{ kEntries__TZ3210_3ulg9kpo, 3 };
constexpr FzConverter kFzDp__TZ3210_3ulg9kpo{
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
    .user_config       = &kMap__TZ3210_3ulg9kpo,
};
const FzConverter* const kFz__TZ3210_3ulg9kpo[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZ3210_3ulg9kpo,
};
constexpr TzConverter kTzDp__TZ3210_3ulg9kpo{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZ3210_3ulg9kpo,
};
const TzConverter* const kTz__TZ3210_3ulg9kpo[] = { &kTzDp__TZ3210_3ulg9kpo };
constexpr const char* kM__TZ3210_3ulg9kpo[] = { "TS0021" };
constexpr const char* kN__TZ3210_3ulg9kpo[] = { "_TZ3210_3ulg9kpo" };
}  // namespace
extern const PreparedDefinition kDefGen__TZ3210_3ulg9kpo{
    .zigbee_models=kM__TZ3210_3ulg9kpo,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZ3210_3ulg9kpo,.manufacturer_names_count=1,
    .model="TS0021__TZ3210_3ulg9kpo",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZ3210_3ulg9kpo,
    .from_zigbee_count=sizeof(kFz__TZ3210_3ulg9kpo)/sizeof(kFz__TZ3210_3ulg9kpo[0]),
    .to_zigbee=kTz__TZ3210_3ulg9kpo,
    .to_zigbee_count=sizeof(kTz__TZ3210_3ulg9kpo)/sizeof(kTz__TZ3210_3ulg9kpo[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
