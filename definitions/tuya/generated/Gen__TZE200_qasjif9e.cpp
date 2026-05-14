// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_qasjif9e).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_qasjif9e[] = {
    { 9, "target_distance", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 12, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 2, "radar_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "maximum_range", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 3, "minimum_range", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 102, "fading_time", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 101, "detection_delay", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_qasjif9e{ kEntries__TZE200_qasjif9e, 8 };
constexpr FzConverter kFzDp__TZE200_qasjif9e{
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
    .user_config       = &kMap__TZE200_qasjif9e,
};
const FzConverter* const kFz__TZE200_qasjif9e[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_qasjif9e,
};
constexpr TzConverter kTzDp__TZE200_qasjif9e{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_qasjif9e,
};
const TzConverter* const kTz__TZE200_qasjif9e[] = { &kTzDp__TZE200_qasjif9e };
constexpr const char* kM__TZE200_qasjif9e[] = { "TS0601" };
constexpr const char* kN__TZE200_qasjif9e[] = { "_TZE200_qasjif9e" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_qasjif9e[] = {
    {"iHseno","TY_24G_Sensor_V2"},
};
extern const PreparedDefinition kDefGen__TZE200_qasjif9e{
    .zigbee_models=kM__TZE200_qasjif9e,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_qasjif9e,.manufacturer_names_count=1,
    .model="TS0601__TZE200_qasjif9e",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_qasjif9e, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_qasjif9e)/sizeof(kWhiteLabels_Gen_TZE200_qasjif9e[0]),
    .from_zigbee=kFz__TZE200_qasjif9e,
    .from_zigbee_count=sizeof(kFz__TZE200_qasjif9e)/sizeof(kFz__TZE200_qasjif9e[0]),
    .to_zigbee=kTz__TZE200_qasjif9e,
    .to_zigbee_count=sizeof(kTz__TZE200_qasjif9e)/sizeof(kTz__TZE200_qasjif9e[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
