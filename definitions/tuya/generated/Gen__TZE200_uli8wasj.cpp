// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_uli8wasj).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_uli8wasj_temperature_unit_dp109[] = {
    { 0, "celsius" },
    { 1, "fahrenheit" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_uli8wasj[] = {
    { 1, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 106, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "fading_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 2, "motion_detection_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 108, "indicator", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 110, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 101, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "temperature_unit", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_uli8wasj_temperature_unit_dp109, sizeof(kEnum__TZE200_uli8wasj_temperature_unit_dp109)/sizeof(kEnum__TZE200_uli8wasj_temperature_unit_dp109[0]) },
    { 105, "temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "humidity_calibration", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 107, "illuminance_interval", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_uli8wasj{ kEntries__TZE200_uli8wasj, 12 };
constexpr FzConverter kFzDp__TZE200_uli8wasj{
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
    .user_config       = &kMap__TZE200_uli8wasj,
};
const FzConverter* const kFz__TZE200_uli8wasj[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_uli8wasj,
};
constexpr TzConverter kTzDp__TZE200_uli8wasj{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_uli8wasj,
};
const TzConverter* const kTz__TZE200_uli8wasj[] = { &kTzDp__TZE200_uli8wasj };
constexpr const char* kM__TZE200_uli8wasj[] = { "TS0601" };
constexpr const char* kN__TZE200_uli8wasj[] = { "_TZE200_uli8wasj" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_uli8wasj{
    .zigbee_models=kM__TZE200_uli8wasj,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_uli8wasj,.manufacturer_names_count=1,
    .model="TS0601__TZE200_uli8wasj",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_uli8wasj,
    .from_zigbee_count=sizeof(kFz__TZE200_uli8wasj)/sizeof(kFz__TZE200_uli8wasj[0]),
    .to_zigbee=kTz__TZE200_uli8wasj,
    .to_zigbee_count=sizeof(kTz__TZE200_uli8wasj)/sizeof(kTz__TZE200_uli8wasj[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
