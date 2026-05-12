// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_gkfbdvyx).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_gkfbdvyx[] = {
    { 2, "move_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "detection_distance_min", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 4, "detection_distance_max", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 9, "distance", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 101, "find_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "presence_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "presence_timeout", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_gkfbdvyx{ kEntries__TZE200_gkfbdvyx, 8 };
constexpr FzConverter kFzDp__TZE200_gkfbdvyx{
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
    .user_config       = &kMap__TZE200_gkfbdvyx,
};
const FzConverter* const kFz__TZE200_gkfbdvyx[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_gkfbdvyx,
};
constexpr TzConverter kTzDp__TZE200_gkfbdvyx{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_gkfbdvyx,
};
const TzConverter* const kTz__TZE200_gkfbdvyx[] = { &kTzDp__TZE200_gkfbdvyx };
constexpr const char* kM__TZE200_gkfbdvyx[] = { "TS0601" };
constexpr const char* kN__TZE200_gkfbdvyx[] = { "_TZE200_gkfbdvyx" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_gkfbdvyx{
    .zigbee_models=kM__TZE200_gkfbdvyx,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_gkfbdvyx,.manufacturer_names_count=1,
    .model="TS0601__TZE200_gkfbdvyx",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_gkfbdvyx,
    .from_zigbee_count=sizeof(kFz__TZE200_gkfbdvyx)/sizeof(kFz__TZE200_gkfbdvyx[0]),
    .to_zigbee=kTz__TZE200_gkfbdvyx,
    .to_zigbee_count=sizeof(kTz__TZE200_gkfbdvyx)/sizeof(kTz__TZE200_gkfbdvyx[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
