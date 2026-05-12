// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_e9ajs4ft).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_e9ajs4ft_indicator_light_dp107[] = {
    { 0, "presence" },
    { 1, "off" },
    { 2, "on" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_e9ajs4ft_detection_method_dp108[] = {
    { 0, "only_move" },
    { 1, "exist_move" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_e9ajs4ft[] = {
    { 1, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "presence_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "detection_range", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 101, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "illuminance_treshold_max", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "illuminance_treshold_min", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "detection_delay", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "presence_illuminance_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 105, "light_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 106, "light_linkage", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 107, "indicator_light", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_e9ajs4ft_indicator_light_dp107, sizeof(kEnum__TZE204_e9ajs4ft_indicator_light_dp107)/sizeof(kEnum__TZE204_e9ajs4ft_indicator_light_dp107[0]) },
    { 108, "detection_method", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_e9ajs4ft_detection_method_dp108, sizeof(kEnum__TZE204_e9ajs4ft_detection_method_dp108)/sizeof(kEnum__TZE204_e9ajs4ft_detection_method_dp108[0]) },
    { 113, "find_switch", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_e9ajs4ft{ kEntries__TZE204_e9ajs4ft, 13 };
constexpr FzConverter kFzDp__TZE204_e9ajs4ft{
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
    .user_config       = &kMap__TZE204_e9ajs4ft,
};
const FzConverter* const kFz__TZE204_e9ajs4ft[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_e9ajs4ft,
};
constexpr TzConverter kTzDp__TZE204_e9ajs4ft{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_e9ajs4ft,
};
const TzConverter* const kTz__TZE204_e9ajs4ft[] = { &kTzDp__TZE204_e9ajs4ft };
constexpr const char* kM__TZE204_e9ajs4ft[] = { "TS0601" };
constexpr const char* kN__TZE204_e9ajs4ft[] = { "_TZE204_e9ajs4ft" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_e9ajs4ft{
    .zigbee_models=kM__TZE204_e9ajs4ft,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_e9ajs4ft,.manufacturer_names_count=1,
    .model="TS0601__TZE204_e9ajs4ft",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_e9ajs4ft,
    .from_zigbee_count=sizeof(kFz__TZE204_e9ajs4ft)/sizeof(kFz__TZE204_e9ajs4ft[0]),
    .to_zigbee=kTz__TZE204_e9ajs4ft,
    .to_zigbee_count=sizeof(kTz__TZE204_e9ajs4ft)/sizeof(kTz__TZE204_e9ajs4ft[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
