// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_juzago6i).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_juzago6i[] = {
    { 1, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 4, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 12, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "detection_distance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "fading_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "static_detection_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "motion_detection_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_juzago6i{ kEntries__TZE200_juzago6i, 7 };
constexpr FzConverter kFzDp__TZE200_juzago6i{
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
    .user_config       = &kMap__TZE200_juzago6i,
};
const FzConverter* const kFz__TZE200_juzago6i[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_juzago6i,
};
constexpr TzConverter kTzDp__TZE200_juzago6i{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_juzago6i,
};
const TzConverter* const kTz__TZE200_juzago6i[] = { &kTzDp__TZE200_juzago6i };
constexpr const char* kM__TZE200_juzago6i[] = { "TS0601" };
constexpr const char* kN__TZE200_juzago6i[] = { "_TZE200_juzago6i" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_juzago6i{
    .zigbee_models=kM__TZE200_juzago6i,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_juzago6i,.manufacturer_names_count=1,
    .model="TS0601__TZE200_juzago6i",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_juzago6i,
    .from_zigbee_count=sizeof(kFz__TZE200_juzago6i)/sizeof(kFz__TZE200_juzago6i[0]),
    .to_zigbee=kTz__TZE200_juzago6i,
    .to_zigbee_count=sizeof(kTz__TZE200_juzago6i)/sizeof(kTz__TZE200_juzago6i[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
