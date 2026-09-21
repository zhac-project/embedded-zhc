// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_tbgecldg).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_tbgecldg_preset_dp2[] = {
    { 0, "auto" },
    { 1, "manual" },
    { 2, "holiday" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_tbgecldg[] = {
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_tbgecldg_preset_dp2, sizeof(kEnum__TZE284_tbgecldg_preset_dp2)/sizeof(kEnum__TZE284_tbgecldg_preset_dp2[0]) },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 2, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 30, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 101, "comfort_temperature", ::zhc::TuyaDpType::Numeric, 2, nullptr, 0, 0 },
    { 102, "eco_temperature", ::zhc::TuyaDpType::Numeric, 2, nullptr, 0, 0 },
    { 103, "holiday_temperature", ::zhc::TuyaDpType::Numeric, 2, nullptr, 0, 0 },
    { 104, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 105, "auto_temperature", ::zhc::TuyaDpType::Numeric, 2, nullptr, 0, 0 },
    { 106, "boost_heating", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 107, "window_open", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 116, "open_window_temperature", ::zhc::TuyaDpType::Numeric, 2, nullptr, 0, 0 },
    { 117, "open_window_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 118, "boost_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_tbgecldg{ kEntries__TZE284_tbgecldg, 14 };
constexpr FzConverter kFzDp__TZE284_tbgecldg{
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
    .user_config       = &kMap__TZE284_tbgecldg,
};
const FzConverter* const kFz__TZE284_tbgecldg[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_tbgecldg,
};
constexpr TzConverter kTzDp__TZE284_tbgecldg{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_tbgecldg,
};
const TzConverter* const kTz__TZE284_tbgecldg[] = { &kTzDp__TZE284_tbgecldg };
constexpr const char* kM__TZE284_tbgecldg[] = { "TS0601" };
constexpr const char* kN__TZE284_tbgecldg[] = { "_TZE284_tbgecldg" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE284_tbgecldg_6[] = {"auto", "manual", "holiday"};
constexpr Expose kAutoExposes__TZE284_tbgecldg[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"comfort_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 0, 29, 0},
    {"eco_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 0, 29, 0},
    {"holiday_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 0, 29, 0},
    {"auto_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 0, 29, 0},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_tbgecldg_6, 3},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -5, 5, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 0, 29, 0},
    {"boost_heating", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"boost_time", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 900, 0},
    {"window_open", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"open_window_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 25, 0},
    {"open_window_time", ExposeType::Numeric, Access::StateSet, "minutes", nullptr, nullptr, 0, ExposeCategory::State, 0, 60, 1},
    {"error_status", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_tbgecldg{
    .zigbee_models=kM__TZE284_tbgecldg,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_tbgecldg,.manufacturer_names_count=1,
    .model="TS0601__TZE284_tbgecldg",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_tbgecldg,.exposes_count=sizeof(kAutoExposes__TZE284_tbgecldg)/sizeof(kAutoExposes__TZE284_tbgecldg[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_tbgecldg,
    .from_zigbee_count=sizeof(kFz__TZE284_tbgecldg)/sizeof(kFz__TZE284_tbgecldg[0]),
    .to_zigbee=kTz__TZE284_tbgecldg,
    .to_zigbee_count=sizeof(kTz__TZE284_tbgecldg)/sizeof(kTz__TZE284_tbgecldg[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
