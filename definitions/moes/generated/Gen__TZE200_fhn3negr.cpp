// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_fhn3negr).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_fhn3negr_preset_dp2[] = {
    { 0, "auto" },
    { 1, "manual" },
    { 2, "holiday" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_fhn3negr[] = {
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_fhn3negr_preset_dp2, sizeof(kEnum__TZE200_fhn3negr_preset_dp2)/sizeof(kEnum__TZE200_fhn3negr_preset_dp2[0]) },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 2, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 30, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 45, "error_status", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "comfort_temperature", ::zhc::TuyaDpType::Numeric, 2, nullptr, 0, 0 },
    { 102, "eco_temperature", ::zhc::TuyaDpType::Numeric, 2, nullptr, 0, 0 },
    { 104, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 105, "auto_setpoint_override", ::zhc::TuyaDpType::Numeric, 2, nullptr, 0, 0 },
    { 106, "boost_heating", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 107, "window_detection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 116, "open_window_temperature", ::zhc::TuyaDpType::Numeric, 2, nullptr, 0, 0 },
    { 117, "window_detection_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 118, "boost_heating_countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 120, "online", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_fhn3negr{ kEntries__TZE200_fhn3negr, 15 };
constexpr FzConverter kFzDp__TZE200_fhn3negr{
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
    .user_config       = &kMap__TZE200_fhn3negr,
};
const FzConverter* const kFz__TZE200_fhn3negr[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_fhn3negr,
};
constexpr TzConverter kTzDp__TZE200_fhn3negr{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_fhn3negr,
};
const TzConverter* const kTz__TZE200_fhn3negr[] = { &kTzDp__TZE200_fhn3negr };
constexpr const char* kM__TZE200_fhn3negr[] = { "TS0601" };
constexpr const char* kN__TZE200_fhn3negr[] = { "_TZE200_fhn3negr" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_fhn3negr_9[] = {"auto", "manual", "holiday"};
constexpr const char* kAutoOpts__TZE200_fhn3negr_10[] = {"idle", "heat"};
constexpr Expose kAutoExposes__TZE200_fhn3negr[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"window_detection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
    {"boost_heating", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"boost_heating_countdown", ExposeType::Numeric, Access::State, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 900, 0},
    {"auto_setpoint_override", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 0, 30, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 0, 30, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -5, 5, 0},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_fhn3negr_9, 3},
    {"running_state", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_fhn3negr_10, 2},
    {"comfort_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 30, 0},
    {"eco_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 30, 0},
    {"open_window_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 30, 0},
    {"window_detection_time", ExposeType::Numeric, Access::StateSet, "min", nullptr, nullptr, 0, ExposeCategory::State, 0, 60, 0},
    {"online", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"error_status", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_fhn3negr{
    .zigbee_models=kM__TZE200_fhn3negr,.zigbee_models_count=sizeof(kM__TZE200_fhn3negr)/sizeof(kM__TZE200_fhn3negr[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_fhn3negr,.manufacturer_names_count=1,
    .model="TS0601__TZE200_fhn3negr",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_fhn3negr,.exposes_count=sizeof(kAutoExposes__TZE200_fhn3negr)/sizeof(kAutoExposes__TZE200_fhn3negr[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_fhn3negr,
    .from_zigbee_count=sizeof(kFz__TZE200_fhn3negr)/sizeof(kFz__TZE200_fhn3negr[0]),
    .to_zigbee=kTz__TZE200_fhn3negr,
    .to_zigbee_count=sizeof(kTz__TZE200_fhn3negr)/sizeof(kTz__TZE200_fhn3negr[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
