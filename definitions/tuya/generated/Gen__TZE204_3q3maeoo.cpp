// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_3q3maeoo).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_3q3maeoo_preset_dp2[] = {
    { 0, "auto" },
    { 1, "manual" },
    { 2, "holiday" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_3q3maeoo_temperature_unit_dp46[] = {
    { 0, "celsius" },
    { 1, "fahrenheit" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_3q3maeoo_auto_mode_dp126[] = {
    { 5, "off" },
    { 4, "antifrost" },
    { 3, "eco" },
    { 2, "comfort_-2" },
    { 1, "comfort_-1" },
    { 0, "comfort" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_3q3maeoo_manual_mode_dp127[] = {
    { 5, "off" },
    { 4, "antifrost" },
    { 3, "eco" },
    { 2, "comfort_-2" },
    { 1, "comfort_-1" },
    { 0, "comfort" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_3q3maeoo_window_opening_mode_dp139[] = {
    { 5, "off" },
    { 4, "antifrost" },
    { 3, "eco" },
    { 2, "comfort_-2" },
    { 1, "comfort_-1" },
    { 0, "comfort" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_3q3maeoo_mode_switching_dp144[] = {
    { 0, "four_modes" },
    { 1, "six_modes" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_3q3maeoo[] = {
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_3q3maeoo_preset_dp2, sizeof(kEnum__TZE204_3q3maeoo_preset_dp2)/sizeof(kEnum__TZE204_3q3maeoo_preset_dp2[0]) },
    { 8, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 11, "energy_consumed", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 39, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 46, "temperature_unit", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_3q3maeoo_temperature_unit_dp46, sizeof(kEnum__TZE204_3q3maeoo_temperature_unit_dp46)/sizeof(kEnum__TZE204_3q3maeoo_temperature_unit_dp46[0]) },
    { 101, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 108, "boost_heating", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 114, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 115, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 116, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 117, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 118, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 119, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 120, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 123, "boost_timeset_countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 126, "auto_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_3q3maeoo_auto_mode_dp126, sizeof(kEnum__TZE204_3q3maeoo_auto_mode_dp126)/sizeof(kEnum__TZE204_3q3maeoo_auto_mode_dp126[0]) },
    { 127, "manual_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_3q3maeoo_manual_mode_dp127, sizeof(kEnum__TZE204_3q3maeoo_manual_mode_dp127)/sizeof(kEnum__TZE204_3q3maeoo_manual_mode_dp127[0]) },
    { 136, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 138, "window_detection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 139, "window_opening_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_3q3maeoo_window_opening_mode_dp139, sizeof(kEnum__TZE204_3q3maeoo_window_opening_mode_dp139)/sizeof(kEnum__TZE204_3q3maeoo_window_opening_mode_dp139[0]) },
    { 140, "window_opening_mode_duration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 141, "reset_consumption", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 142, "power_rating", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 144, "mode_switching", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_3q3maeoo_mode_switching_dp144, sizeof(kEnum__TZE204_3q3maeoo_mode_switching_dp144)/sizeof(kEnum__TZE204_3q3maeoo_mode_switching_dp144[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_3q3maeoo{ kEntries__TZE204_3q3maeoo, 25 };
constexpr FzConverter kFzDp__TZE204_3q3maeoo{
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
    .user_config       = &kMap__TZE204_3q3maeoo,
};
const FzConverter* const kFz__TZE204_3q3maeoo[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_3q3maeoo,
};
constexpr TzConverter kTzDp__TZE204_3q3maeoo{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_3q3maeoo,
};
const TzConverter* const kTz__TZE204_3q3maeoo[] = { &kTzDp__TZE204_3q3maeoo };
constexpr const char* kM__TZE204_3q3maeoo[] = { "TS0601" };
constexpr const char* kN__TZE204_3q3maeoo[] = { "_TZE204_3q3maeoo" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_3q3maeoo{
    .zigbee_models=kM__TZE204_3q3maeoo,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_3q3maeoo,.manufacturer_names_count=1,
    .model="TS0601__TZE204_3q3maeoo",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_3q3maeoo,
    .from_zigbee_count=sizeof(kFz__TZE204_3q3maeoo)/sizeof(kFz__TZE204_3q3maeoo[0]),
    .to_zigbee=kTz__TZE204_3q3maeoo,
    .to_zigbee_count=sizeof(kTz__TZE204_3q3maeoo)/sizeof(kTz__TZE204_3q3maeoo[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
