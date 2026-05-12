// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_tyffvoij).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_tyffvoij_motion_state_dp101[] = {
    { 0, "none" },
    { 1, "large" },
    { 2, "small" },
    { 3, "static" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_tyffvoij_motion_detection_mode_dp122[] = {
    { 0, "only_pir" },
    { 1, "pir_and_radar" },
    { 2, "only_radar" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_tyffvoij[] = {
    { 1, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "static_detection_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "static_detection_distance", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 101, "motion_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_tyffvoij_motion_state_dp101, sizeof(kEnum__TZE200_tyffvoij_motion_state_dp101)/sizeof(kEnum__TZE200_tyffvoij_motion_state_dp101[0]) },
    { 102, "fading_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "indicator", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 121, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 122, "motion_detection_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_tyffvoij_motion_detection_mode_dp122, sizeof(kEnum__TZE200_tyffvoij_motion_detection_mode_dp122)/sizeof(kEnum__TZE200_tyffvoij_motion_detection_mode_dp122[0]) },
    { 123, "motion_detection_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_tyffvoij{ kEntries__TZE200_tyffvoij, 10 };
constexpr FzConverter kFzDp__TZE200_tyffvoij{
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
    .user_config       = &kMap__TZE200_tyffvoij,
};
const FzConverter* const kFz__TZE200_tyffvoij[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_tyffvoij,
};
constexpr TzConverter kTzDp__TZE200_tyffvoij{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_tyffvoij,
};
const TzConverter* const kTz__TZE200_tyffvoij[] = { &kTzDp__TZE200_tyffvoij };
constexpr const char* kM__TZE200_tyffvoij[] = { "TS0601" };
constexpr const char* kN__TZE200_tyffvoij[] = { "_TZE200_tyffvoij" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_tyffvoij{
    .zigbee_models=kM__TZE200_tyffvoij,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_tyffvoij,.manufacturer_names_count=1,
    .model="TS0601__TZE200_tyffvoij",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_tyffvoij,
    .from_zigbee_count=sizeof(kFz__TZE200_tyffvoij)/sizeof(kFz__TZE200_tyffvoij[0]),
    .to_zigbee=kTz__TZE200_tyffvoij,
    .to_zigbee_count=sizeof(kTz__TZE200_tyffvoij)/sizeof(kTz__TZE200_tyffvoij[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
