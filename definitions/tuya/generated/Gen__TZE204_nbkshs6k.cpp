// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_nbkshs6k).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_nbkshs6k_sensitivity_dp9[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_nbkshs6k_keep_time_dp10[] = {
    { 0, "30" },
    { 1, "60" },
    { 2, "120" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_nbkshs6k[] = {
    { 9, "sensitivity", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_nbkshs6k_sensitivity_dp9, sizeof(kEnum__TZE204_nbkshs6k_sensitivity_dp9)/sizeof(kEnum__TZE204_nbkshs6k_sensitivity_dp9[0]) },
    { 10, "keep_time", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_nbkshs6k_keep_time_dp10, sizeof(kEnum__TZE204_nbkshs6k_keep_time_dp10)/sizeof(kEnum__TZE204_nbkshs6k_keep_time_dp10[0]) },
    { 12, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_nbkshs6k{ kEntries__TZE204_nbkshs6k, 3 };
constexpr FzConverter kFzDp__TZE204_nbkshs6k{
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
    .user_config       = &kMap__TZE204_nbkshs6k,
};
const FzConverter* const kFz__TZE204_nbkshs6k[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_nbkshs6k,
};
constexpr TzConverter kTzDp__TZE204_nbkshs6k{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_nbkshs6k,
};
const TzConverter* const kTz__TZE204_nbkshs6k[] = { &kTzDp__TZE204_nbkshs6k };
constexpr const char* kM__TZE204_nbkshs6k[] = { "TS0601" };
constexpr const char* kN__TZE204_nbkshs6k[] = { "_TZE204_nbkshs6k" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_nbkshs6k{
    .zigbee_models=kM__TZE204_nbkshs6k,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_nbkshs6k,.manufacturer_names_count=1,
    .model="TS0601__TZE204_nbkshs6k",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_nbkshs6k,
    .from_zigbee_count=sizeof(kFz__TZE204_nbkshs6k)/sizeof(kFz__TZE204_nbkshs6k[0]),
    .to_zigbee=kTz__TZE204_nbkshs6k,
    .to_zigbee_count=sizeof(kTz__TZE204_nbkshs6k)/sizeof(kTz__TZE204_nbkshs6k[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
