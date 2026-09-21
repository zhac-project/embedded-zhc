// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_zxkwaztm).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_zxkwaztm_running_state_dp36[] = {
    { 2, "cool" },
    { 1, "heat" },
    { 0, "idle" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_zxkwaztm_preset_dp2[] = {
    { 0, "schedule" },
    { 1, "manual" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_zxkwaztm_working_day_dp31[] = {
    { 0, "mon_fri" },
    { 1, "mon_sat" },
    { 2, "mon_sun" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_zxkwaztm[] = {
    { 36, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_zxkwaztm_running_state_dp36, sizeof(kEnum__TZE204_zxkwaztm_running_state_dp36)/sizeof(kEnum__TZE204_zxkwaztm_running_state_dp36[0]) },
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_zxkwaztm_preset_dp2, sizeof(kEnum__TZE204_zxkwaztm_preset_dp2)/sizeof(kEnum__TZE204_zxkwaztm_preset_dp2[0]) },
    { 10, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 40, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 109, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 112, "temperature_delta", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 31, "working_day", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_zxkwaztm_working_day_dp31, sizeof(kEnum__TZE204_zxkwaztm_working_day_dp31)/sizeof(kEnum__TZE204_zxkwaztm_working_day_dp31[0]) },
    { 67, "schedule_weekday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 68, "schedule_holiday", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_zxkwaztm{ kEntries__TZE204_zxkwaztm, 11 };
constexpr FzConverter kFzDp__TZE204_zxkwaztm{
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
    .user_config       = &kMap__TZE204_zxkwaztm,
};
const FzConverter* const kFz__TZE204_zxkwaztm[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_zxkwaztm,
};
constexpr TzConverter kTzDp__TZE204_zxkwaztm{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_zxkwaztm,
};
const TzConverter* const kTz__TZE204_zxkwaztm[] = { &kTzDp__TZE204_zxkwaztm };
constexpr const char* kM__TZE204_zxkwaztm[] = { "TS0601" };
constexpr const char* kN__TZE204_zxkwaztm[] = { "_TZE204_zxkwaztm" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE204_zxkwaztm_1[] = {"off", "heat"};
constexpr const char* kAutoOpts__TZE204_zxkwaztm_3[] = {"idle", "heat"};
constexpr const char* kAutoOpts__TZE204_zxkwaztm_4[] = {"schedule", "manual"};
constexpr const char* kAutoOpts__TZE204_zxkwaztm_8[] = {"mon_fri", "mon_sat", "mon_sun"};
constexpr Expose kAutoExposes__TZE204_zxkwaztm[] = {
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_zxkwaztm_1, 2},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"running_state", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_zxkwaztm_3, 2},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_zxkwaztm_4, 2},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -9, 9, 0},
    {"temperature_delta", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 0, 10, 0},
    {"working_day", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_zxkwaztm_8, 3},
    {"schedule_weekday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_holiday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"frost_protection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_zxkwaztm{
    .zigbee_models=kM__TZE204_zxkwaztm,.zigbee_models_count=sizeof(kM__TZE204_zxkwaztm)/sizeof(kM__TZE204_zxkwaztm[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_zxkwaztm,.manufacturer_names_count=1,
    .model="TS0601__TZE204_zxkwaztm",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_zxkwaztm,.exposes_count=sizeof(kAutoExposes__TZE204_zxkwaztm)/sizeof(kAutoExposes__TZE204_zxkwaztm[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_zxkwaztm,
    .from_zigbee_count=sizeof(kFz__TZE204_zxkwaztm)/sizeof(kFz__TZE204_zxkwaztm[0]),
    .to_zigbee=kTz__TZE204_zxkwaztm,
    .to_zigbee_count=sizeof(kTz__TZE204_zxkwaztm)/sizeof(kTz__TZE204_zxkwaztm[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
