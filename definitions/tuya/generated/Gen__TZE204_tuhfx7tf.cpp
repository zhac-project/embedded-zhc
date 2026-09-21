// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_tuhfx7tf).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_tuhfx7tf_over_voltage_setting_dp102[] = {
    { 0, "Ignore" },
    { 1, "Alarm" },
    { 2, "Trip" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_tuhfx7tf_under_voltage_setting_dp103[] = {
    { 0, "Ignore" },
    { 1, "Alarm" },
    { 2, "Trip" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_tuhfx7tf_over_current_setting_dp104[] = {
    { 0, "Ignore" },
    { 1, "Alarm" },
    { 2, "Trip" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_tuhfx7tf_over_power_setting_dp105[] = {
    { 0, "Ignore" },
    { 1, "Alarm" },
    { 2, "Trip" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_tuhfx7tf_temperature_setting_dp107[] = {
    { 0, "Ignore" },
    { 1, "Alarm" },
    { 2, "Trip" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_tuhfx7tf_leakage_setting_dp108[] = {
    { 0, "Ignore" },
    { 1, "Alarm" },
    { 2, "Trip" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_tuhfx7tf[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 15, "leakage_current", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "over_voltage_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_tuhfx7tf_over_voltage_setting_dp102, sizeof(kEnum__TZE204_tuhfx7tf_over_voltage_setting_dp102)/sizeof(kEnum__TZE204_tuhfx7tf_over_voltage_setting_dp102[0]) },
    { 103, "under_voltage_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_tuhfx7tf_under_voltage_setting_dp103, sizeof(kEnum__TZE204_tuhfx7tf_under_voltage_setting_dp103)/sizeof(kEnum__TZE204_tuhfx7tf_under_voltage_setting_dp103[0]) },
    { 104, "over_current_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_tuhfx7tf_over_current_setting_dp104, sizeof(kEnum__TZE204_tuhfx7tf_over_current_setting_dp104)/sizeof(kEnum__TZE204_tuhfx7tf_over_current_setting_dp104[0]) },
    { 105, "over_power_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_tuhfx7tf_over_power_setting_dp105, sizeof(kEnum__TZE204_tuhfx7tf_over_power_setting_dp105)/sizeof(kEnum__TZE204_tuhfx7tf_over_power_setting_dp105[0]) },
    { 107, "temperature_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_tuhfx7tf_temperature_setting_dp107, sizeof(kEnum__TZE204_tuhfx7tf_temperature_setting_dp107)/sizeof(kEnum__TZE204_tuhfx7tf_temperature_setting_dp107[0]) },
    { 108, "leakage_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_tuhfx7tf_leakage_setting_dp108, sizeof(kEnum__TZE204_tuhfx7tf_leakage_setting_dp108)/sizeof(kEnum__TZE204_tuhfx7tf_leakage_setting_dp108[0]) },
    { 112, "auto_reclosing", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 113, "restore_default", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 114, "over_current_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 115, "over_voltage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 116, "under_voltage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 117, "leakage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 118, "temperature_threshold", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 119, "over_power_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 131, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 143, "overcurrent_recloser", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 144, "leakage_recloser", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 145, "overpower_recloser", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_tuhfx7tf{ kEntries__TZE204_tuhfx7tf, 21 };
constexpr FzConverter kFzDp__TZE204_tuhfx7tf{
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
    .user_config       = &kMap__TZE204_tuhfx7tf,
};
const FzConverter* const kFz__TZE204_tuhfx7tf[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_tuhfx7tf,
};
constexpr TzConverter kTzDp__TZE204_tuhfx7tf{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_tuhfx7tf,
};
const TzConverter* const kTz__TZE204_tuhfx7tf[] = { &kTzDp__TZE204_tuhfx7tf };
constexpr const char* kM__TZE204_tuhfx7tf[] = { "TS0601" };
constexpr const char* kN__TZE204_tuhfx7tf[] = { "_TZE204_tuhfx7tf" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE204_tuhfx7tf_6[] = {"normal", "over_current_trip", "over_power_trip", "high_temp_trip", "over_voltage_trip", "under_voltage_trip", "over_current_alarm", "over_power_alarm", "high_temp_alarm", "over_voltage_alarm", "under_voltage_alarm", "remote_on", "remote_off", "manual_on", "manual_off", "leakage_trip", "leakage_alarm", "restore_default", "automatic_closing", "electricity_shortage", "electricity_shortage_alarm", "timing_switch_On", "timing_switch_off"};
constexpr const char* kAutoOpts__TZE204_tuhfx7tf_7[] = {"Ignore", "Alarm", "Trip"};
constexpr const char* kAutoOpts__TZE204_tuhfx7tf_9[] = {"Ignore", "Alarm", "Trip"};
constexpr const char* kAutoOpts__TZE204_tuhfx7tf_11[] = {"Ignore", "Alarm", "Trip"};
constexpr const char* kAutoOpts__TZE204_tuhfx7tf_13[] = {"Ignore", "Alarm", "Trip"};
constexpr Expose kAutoExposes__TZE204_tuhfx7tf[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"event", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_tuhfx7tf_6, 23},
    {"over_voltage_setting", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_tuhfx7tf_7, 3},
    {"over_voltage_threshold", ExposeType::Numeric, Access::StateSet, "V", nullptr, nullptr, 0, ExposeCategory::State, 240, 295, 0},
    {"under_voltage_setting", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_tuhfx7tf_9, 3},
    {"under_voltage_threshold", ExposeType::Numeric, Access::StateSet, "V", nullptr, nullptr, 0, ExposeCategory::State, 145, 220, 0},
    {"temperature_setting", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_tuhfx7tf_11, 3},
    {"temperature_threshold", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -25, 100, 0},
    {"over_power_setting", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_tuhfx7tf_13, 3},
    {"over_power_threshold", ExposeType::Numeric, Access::StateSet, "W", nullptr, nullptr, 0, ExposeCategory::State, 5, 25000, 10},
    {"auto_reclosing", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"restore_default", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"overcurrent_recloser", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"leakage_recloser", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"overpower_recloser", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_tuhfx7tf[] = {
    {"Tongou","TOWSMR1-40A-AC"},
    {"Tongou","TOWSMR1-40A-A"},
    {"Tongou","TOWSMR1-20A-AC"},
};
extern const PreparedDefinition kDefGen__TZE204_tuhfx7tf{
    .zigbee_models=kM__TZE204_tuhfx7tf,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_tuhfx7tf,.manufacturer_names_count=1,
    .model="TS0601__TZE204_tuhfx7tf",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_tuhfx7tf,.exposes_count=sizeof(kAutoExposes__TZE204_tuhfx7tf)/sizeof(kAutoExposes__TZE204_tuhfx7tf[0]),
    .white_labels=kWhiteLabels_Gen_TZE204_tuhfx7tf, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_tuhfx7tf)/sizeof(kWhiteLabels_Gen_TZE204_tuhfx7tf[0]),
    .from_zigbee=kFz__TZE204_tuhfx7tf,
    .from_zigbee_count=sizeof(kFz__TZE204_tuhfx7tf)/sizeof(kFz__TZE204_tuhfx7tf[0]),
    .to_zigbee=kTz__TZE204_tuhfx7tf,
    .to_zigbee_count=sizeof(kTz__TZE204_tuhfx7tf)/sizeof(kTz__TZE204_tuhfx7tf[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
