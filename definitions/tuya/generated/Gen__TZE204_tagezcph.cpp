// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_tagezcph).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_tagezcph_preset_dp2[] = {
    { 0, "auto" },
    { 1, "manual" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_tagezcph_running_state_dp36[] = {
    { 1, "idle" },
    { 0, "heat" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_tagezcph_sensor_dp43[] = {
    { 0, "IN" },
    { 2, "OU" },
    { 1, "AL" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_tagezcph[] = {
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_tagezcph_preset_dp2, sizeof(kEnum__TZE204_tagezcph_preset_dp2)/sizeof(kEnum__TZE204_tagezcph_preset_dp2[0]) },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 19, "max_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 26, "min_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 27, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 36, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_tagezcph_running_state_dp36, sizeof(kEnum__TZE204_tagezcph_running_state_dp36)/sizeof(kEnum__TZE204_tagezcph_running_state_dp36[0]) },
    { 43, "sensor", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_tagezcph_sensor_dp43, sizeof(kEnum__TZE204_tagezcph_sensor_dp43)/sizeof(kEnum__TZE204_tagezcph_sensor_dp43[0]) },
    { 102, "external_temperature_input", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 103, "deadzone_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "max_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 101, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 105, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 106, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 107, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 108, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 109, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 110, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 111, "min_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 112, "eco_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 113, "brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 114, "display_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_tagezcph{ kEntries__TZE204_tagezcph, 22 };
constexpr FzConverter kFzDp__TZE204_tagezcph{
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
    .user_config       = &kMap__TZE204_tagezcph,
};
const FzConverter* const kFz__TZE204_tagezcph[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_tagezcph,
};
constexpr TzConverter kTzDp__TZE204_tagezcph{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_tagezcph,
};
const TzConverter* const kTz__TZE204_tagezcph[] = { &kTzDp__TZE204_tagezcph };
constexpr const char* kM__TZE204_tagezcph[] = { "TS0601" };
constexpr const char* kN__TZE204_tagezcph[] = { "_TZE204_tagezcph" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_tagezcph{
    .zigbee_models=kM__TZE204_tagezcph,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_tagezcph,.manufacturer_names_count=1,
    .model="TS0601__TZE204_tagezcph",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_tagezcph,
    .from_zigbee_count=sizeof(kFz__TZE204_tagezcph)/sizeof(kFz__TZE204_tagezcph[0]),
    .to_zigbee=kTz__TZE204_tagezcph,
    .to_zigbee_count=sizeof(kTz__TZE204_tagezcph)/sizeof(kTz__TZE204_tagezcph[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
