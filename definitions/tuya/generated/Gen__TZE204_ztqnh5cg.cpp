// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_ztqnh5cg).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_ztqnh5cg[] = {
    { 9, "target_distance", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 12, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 2, "radar_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "maximum_range", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 3, "minimum_range", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 102, "fading_time", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 101, "detection_delay", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_ztqnh5cg{ kEntries__TZE204_ztqnh5cg, 8 };
constexpr FzConverter kFzDp__TZE204_ztqnh5cg{
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
    .user_config       = &kMap__TZE204_ztqnh5cg,
};
const FzConverter* const kFz__TZE204_ztqnh5cg[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_ztqnh5cg,
};
constexpr TzConverter kTzDp__TZE204_ztqnh5cg{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_ztqnh5cg,
};
const TzConverter* const kTz__TZE204_ztqnh5cg[] = { &kTzDp__TZE204_ztqnh5cg };
constexpr const char* kM__TZE204_ztqnh5cg[] = { "TS0601" };
constexpr const char* kN__TZE204_ztqnh5cg[] = { "_TZE204_ztqnh5cg" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_ztqnh5cg[] = {
    {"iHseno","TY_24G_Sensor_V2"},
};
extern const PreparedDefinition kDefGen__TZE204_ztqnh5cg{
    .zigbee_models=kM__TZE204_ztqnh5cg,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_ztqnh5cg,.manufacturer_names_count=1,
    .model="TS0601__TZE204_ztqnh5cg",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_ztqnh5cg, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_ztqnh5cg)/sizeof(kWhiteLabels_Gen_TZE204_ztqnh5cg[0]),
    .from_zigbee=kFz__TZE204_ztqnh5cg,
    .from_zigbee_count=sizeof(kFz__TZE204_ztqnh5cg)/sizeof(kFz__TZE204_ztqnh5cg[0]),
    .to_zigbee=kTz__TZE204_ztqnh5cg,
    .to_zigbee_count=sizeof(kTz__TZE204_ztqnh5cg)/sizeof(kTz__TZE204_ztqnh5cg[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
