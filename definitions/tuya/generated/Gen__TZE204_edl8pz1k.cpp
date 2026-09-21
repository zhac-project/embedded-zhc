// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_edl8pz1k).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_edl8pz1k_preset_dp2[] = {
    { 0, "schedule" },
    { 1, "manual" },
    { 2, "away" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_edl8pz1k_running_state_dp36[] = {
    { 0, "heat" },
    { 1, "idle" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_edl8pz1k[] = {
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_edl8pz1k_preset_dp2, sizeof(kEnum__TZE204_edl8pz1k_preset_dp2)/sizeof(kEnum__TZE204_edl8pz1k_preset_dp2[0]) },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 24, "device_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 27, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 36, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_edl8pz1k_running_state_dp36, sizeof(kEnum__TZE204_edl8pz1k_running_state_dp36)/sizeof(kEnum__TZE204_edl8pz1k_running_state_dp36[0]) },
    { 40, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 103, "deadzone_temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 110, "schedule_monday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 109, "schedule_tuesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 108, "schedule_wednesday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 107, "schedule_thursday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 106, "schedule_friday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 105, "schedule_saturday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
    { 101, "schedule_sunday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagScheduleDay },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_edl8pz1k{ kEntries__TZE204_edl8pz1k, 15 };
constexpr FzConverter kFzDp__TZE204_edl8pz1k{
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
    .user_config       = &kMap__TZE204_edl8pz1k,
};
const FzConverter* const kFz__TZE204_edl8pz1k[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_edl8pz1k,
};
constexpr TzConverter kTzDp__TZE204_edl8pz1k{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_edl8pz1k,
};
const TzConverter* const kTz__TZE204_edl8pz1k[] = { &kTzDp__TZE204_edl8pz1k };
constexpr const char* kM__TZE204_edl8pz1k[] = { "TS0601" };
constexpr const char* kN__TZE204_edl8pz1k[] = { "_TZE204_edl8pz1k" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE204_edl8pz1k_0[] = {"off", "heat"};
constexpr const char* kAutoOpts__TZE204_edl8pz1k_1[] = {"manual", "auto"};
constexpr const char* kAutoOpts__TZE204_edl8pz1k_2[] = {"idle", "heat"};
constexpr Expose kAutoExposes__TZE204_edl8pz1k[] = {
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_edl8pz1k_0, 2},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_edl8pz1k_1, 2},
    {"running_state", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_edl8pz1k_2, 2},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -9, 9, 0},
    {"deadzone_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 0, 5, 1},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_monday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_tuesday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_wednesday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_thursday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_friday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_saturday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_sunday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_edl8pz1k[] = {
    {"ELECTSMART","EST-120Z"},
};
extern const PreparedDefinition kDefGen__TZE204_edl8pz1k{
    .zigbee_models=kM__TZE204_edl8pz1k,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_edl8pz1k,.manufacturer_names_count=1,
    .model="TS0601__TZE204_edl8pz1k",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_edl8pz1k,.exposes_count=sizeof(kAutoExposes__TZE204_edl8pz1k)/sizeof(kAutoExposes__TZE204_edl8pz1k[0]),
    .white_labels=kWhiteLabels_Gen_TZE204_edl8pz1k, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_edl8pz1k)/sizeof(kWhiteLabels_Gen_TZE204_edl8pz1k[0]),
    .from_zigbee=kFz__TZE204_edl8pz1k,
    .from_zigbee_count=sizeof(kFz__TZE204_edl8pz1k)/sizeof(kFz__TZE204_edl8pz1k[0]),
    .to_zigbee=kTz__TZE204_edl8pz1k,
    .to_zigbee_count=sizeof(kTz__TZE204_edl8pz1k)/sizeof(kTz__TZE204_edl8pz1k[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
