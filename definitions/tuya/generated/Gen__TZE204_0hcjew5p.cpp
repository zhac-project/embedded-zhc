// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_0hcjew5p).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_0hcjew5p_mode_dp2[] = {
    { 0, "comfort" },
    { 1, "eco" },
    { 2, "antifrost" },
    { 3, "off" },
    { 4, "comfort_1" },
    { 5, "comfort_2" },
    { 6, "program" },
    { 7, "manual" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_0hcjew5p_preset_dp2[] = {
    { 0, "comfort" },
    { 1, "eco" },
    { 2, "antifrost" },
    { 3, "off" },
    { 4, "comfort_1" },
    { 5, "comfort_2" },
    { 6, "program" },
    { 7, "manual" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_0hcjew5p_window_dp17[] = {
    { 0, "close" },
    { 1, "open" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_0hcjew5p_device_mode_type_dp106[] = {
    { 0, "four" },
    { 1, "six" },
    { 2, "switch" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_0hcjew5p[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_0hcjew5p_mode_dp2, sizeof(kEnum__TZE204_0hcjew5p_mode_dp2)/sizeof(kEnum__TZE204_0hcjew5p_mode_dp2[0]) },
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_0hcjew5p_preset_dp2, sizeof(kEnum__TZE204_0hcjew5p_preset_dp2)/sizeof(kEnum__TZE204_0hcjew5p_preset_dp2[0]) },
    { 11, "power", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 16, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 17, "window", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_0hcjew5p_window_dp17, sizeof(kEnum__TZE204_0hcjew5p_window_dp17)/sizeof(kEnum__TZE204_0hcjew5p_window_dp17[0]) },
    { 19, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 20, "fault", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 29, "window_detection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 39, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 50, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 101, "voltage", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 102, "current", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 103, "temperature_sensibility", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 104, "energy_today", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 105, "energy_yesterday", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 106, "device_mode_type", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_0hcjew5p_device_mode_type_dp106, sizeof(kEnum__TZE204_0hcjew5p_device_mode_type_dp106)/sizeof(kEnum__TZE204_0hcjew5p_device_mode_type_dp106[0]) },
    { 107, "energy", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 108, "week_program_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "week_program_2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 110, "week_program_3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "week_program_4", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 112, "week_program_5", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 113, "week_program_6", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 114, "week_program_7", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 115, "set_temp_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 115, "radiators_without_integrated_regulation", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 116, "antifrost", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 117, "eco_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 118, "comfort_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 119, "antifrost_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 120, "light", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_0hcjew5p{ kEntries__TZE204_0hcjew5p, 32 };
constexpr FzConverter kFzDp__TZE204_0hcjew5p{
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
    .user_config       = &kMap__TZE204_0hcjew5p,
};
const FzConverter* const kFz__TZE204_0hcjew5p[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_0hcjew5p,
};
constexpr TzConverter kTzDp__TZE204_0hcjew5p{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_0hcjew5p,
};
const TzConverter* const kTz__TZE204_0hcjew5p[] = { &kTzDp__TZE204_0hcjew5p };
constexpr const char* kM__TZE204_0hcjew5p[] = { "TS0601" };
constexpr const char* kN__TZE204_0hcjew5p[] = { "_TZE204_0hcjew5p" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_0hcjew5p{
    .zigbee_models=kM__TZE204_0hcjew5p,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_0hcjew5p,.manufacturer_names_count=1,
    .model="TS0601__TZE204_0hcjew5p",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_0hcjew5p,
    .from_zigbee_count=sizeof(kFz__TZE204_0hcjew5p)/sizeof(kFz__TZE204_0hcjew5p[0]),
    .to_zigbee=kTz__TZE204_0hcjew5p,
    .to_zigbee_count=sizeof(kTz__TZE204_0hcjew5p)/sizeof(kTz__TZE204_0hcjew5p[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
