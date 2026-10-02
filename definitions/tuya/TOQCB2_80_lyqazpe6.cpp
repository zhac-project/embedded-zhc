// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tongou TOQCB2-80 circuit breaker for _TZE204_lyqazpe6 / _TZE284_lyqazpe6
// (z2m v26.115.1 window). The family lives in per-manufacturer generated
// copies, identical apart from the manufacturer; this is a hand-written copy of
// _TZE204_mrffaamu's for the new members, under z2m's model name.
// z2m-source: tuya.ts #TOQCB2-80.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_lyqazpe6_over_voltage_setting_dp102[] = {
    { 0, "closed" },
    { 1, "alarm" },
    { 2, "trip" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_lyqazpe6_under_voltage_setting_dp103[] = {
    { 0, "closed" },
    { 1, "alarm" },
    { 2, "trip" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_lyqazpe6_over_current_setting_dp104[] = {
    { 0, "closed" },
    { 1, "alarm" },
    { 2, "trip" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_lyqazpe6_over_power_setting_dp105[] = {
    { 0, "closed" },
    { 1, "alarm" },
    { 2, "trip" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_lyqazpe6_temperature_setting_dp107[] = {
    { 0, "closed" },
    { 1, "alarm" },
    { 2, "trip" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_lyqazpe6_last_event_dp110[] = {
    { 0, "normal" },
    { 1, "trip_over_current" },
    { 2, "trip_over_power" },
    { 3, "trip_over_temperature" },
    { 4, "trip_voltage_1" },
    { 5, "trip_voltage_2" },
    { 6, "alarm_over_current" },
    { 7, "alarm_over_power" },
    { 8, "alarm_over_temperature" },
    { 9, "alarm_voltage_1" },
    { 10, "alarm_voltage_2" },
    { 11, "remote_on" },
    { 12, "remote_off" },
    { 13, "manual_on" },
    { 14, "manual_off" },
    { 15, "value_15" },
    { 16, "value_16" },
    { 17, "factory_reset" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_lyqazpe6[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 16, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "over_voltage_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_lyqazpe6_over_voltage_setting_dp102, sizeof(kEnum__TZE204_lyqazpe6_over_voltage_setting_dp102)/sizeof(kEnum__TZE204_lyqazpe6_over_voltage_setting_dp102[0]) },
    { 103, "under_voltage_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_lyqazpe6_under_voltage_setting_dp103, sizeof(kEnum__TZE204_lyqazpe6_under_voltage_setting_dp103)/sizeof(kEnum__TZE204_lyqazpe6_under_voltage_setting_dp103[0]) },
    { 104, "over_current_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_lyqazpe6_over_current_setting_dp104, sizeof(kEnum__TZE204_lyqazpe6_over_current_setting_dp104)/sizeof(kEnum__TZE204_lyqazpe6_over_current_setting_dp104[0]) },
    { 105, "over_power_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_lyqazpe6_over_power_setting_dp105, sizeof(kEnum__TZE204_lyqazpe6_over_power_setting_dp105)/sizeof(kEnum__TZE204_lyqazpe6_over_power_setting_dp105[0]) },
    { 107, "temperature_setting", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_lyqazpe6_temperature_setting_dp107, sizeof(kEnum__TZE204_lyqazpe6_temperature_setting_dp107)/sizeof(kEnum__TZE204_lyqazpe6_temperature_setting_dp107[0]) },
    { 110, "last_event", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_lyqazpe6_last_event_dp110, sizeof(kEnum__TZE204_lyqazpe6_last_event_dp110)/sizeof(kEnum__TZE204_lyqazpe6_last_event_dp110[0]) },
    { 112, "clear_fault", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 113, "factory_reset", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 114, "current_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 115, "over_voltage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 116, "under_voltage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 118, "temperature_threshold", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 119, "over_power_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 131, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_lyqazpe6{ kEntries__TZE204_lyqazpe6, sizeof(kEntries__TZE204_lyqazpe6)/sizeof(kEntries__TZE204_lyqazpe6[0]) };
constexpr FzConverter kFzDp__TZE204_lyqazpe6{
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
    .user_config       = &kMap__TZE204_lyqazpe6,
};
const FzConverter* const kFz__TZE204_lyqazpe6[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_lyqazpe6,
};
constexpr TzConverter kTzDp__TZE204_lyqazpe6{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_lyqazpe6,
};
const TzConverter* const kTz__TZE204_lyqazpe6[] = { &kTzDp__TZE204_lyqazpe6 };
constexpr const char* kM__TZE204_lyqazpe6[] = { "TS0601" };
constexpr const char* kN__TZE204_lyqazpe6[] = { "_TZE204_lyqazpe6", "_TZE284_lyqazpe6" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE204_lyqazpe6_15[] = {"normal", "trip_over_current", "trip_over_power", "trip_over_temperature", "trip_voltage_1", "trip_voltage_2", "alarm_over_current", "alarm_over_power", "alarm_over_temperature", "alarm_voltage_1", "alarm_voltage_2", "remote_on", "remote_off", "manual_on", "manual_off", "value_15", "value_16", "factory_reset"};
constexpr const char* kAutoOpts__TZE204_lyqazpe6_16[] = {"closed", "alarm", "trip"};
constexpr const char* kAutoOpts__TZE204_lyqazpe6_18[] = {"closed", "alarm", "trip"};
constexpr const char* kAutoOpts__TZE204_lyqazpe6_20[] = {"closed", "alarm", "trip"};
constexpr const char* kAutoOpts__TZE204_lyqazpe6_22[] = {"closed", "alarm", "trip"};
constexpr const char* kAutoOpts__TZE204_lyqazpe6_24[] = {"closed", "alarm", "trip"};
constexpr Expose kAutoExposes__TZE204_lyqazpe6[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"voltage_a", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"voltage_b", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"voltage_c", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"power_a", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_b", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_c", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"current_a", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_b", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_c", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"last_event", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_lyqazpe6_15, 18},
    {"over_current_setting", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_lyqazpe6_16, 3},
    {"current_threshold", ExposeType::Numeric, Access::StateSet, "A", nullptr, nullptr, 0, ExposeCategory::State, 1, 63, 1},
    {"under_voltage_setting", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_lyqazpe6_18, 3},
    {"under_voltage_threshold", ExposeType::Numeric, Access::StateSet, "V", nullptr, nullptr, 0, ExposeCategory::State, 145, 220, 1},
    {"over_voltage_setting", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_lyqazpe6_20, 3},
    {"over_voltage_threshold", ExposeType::Numeric, Access::StateSet, "V", nullptr, nullptr, 0, ExposeCategory::State, 245, 295, 1},
    {"over_power_setting", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_lyqazpe6_22, 3},
    {"over_power_threshold", ExposeType::Numeric, Access::StateSet, "W", nullptr, nullptr, 0, ExposeCategory::State, 200, 20000, 100},
    {"temperature_setting", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_lyqazpe6_24, 3},
    {"temperature_threshold", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -40, 100, 1},
    {"clear_fault", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"factory_reset", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"auto_reclosing", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDef_TOQCB2_80_lyqazpe6{
    .zigbee_models=kM__TZE204_lyqazpe6,.zigbee_models_count=sizeof(kM__TZE204_lyqazpe6)/sizeof(kM__TZE204_lyqazpe6[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_lyqazpe6,.manufacturer_names_count=sizeof(kN__TZE204_lyqazpe6)/sizeof(kN__TZE204_lyqazpe6[0]),
    .model="TOQCB2-80",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_lyqazpe6,.exposes_count=sizeof(kAutoExposes__TZE204_lyqazpe6)/sizeof(kAutoExposes__TZE204_lyqazpe6[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_lyqazpe6,
    .from_zigbee_count=sizeof(kFz__TZE204_lyqazpe6)/sizeof(kFz__TZE204_lyqazpe6[0]),
    .to_zigbee=kTz__TZE204_lyqazpe6,
    .to_zigbee_count=sizeof(kTz__TZE204_lyqazpe6)/sizeof(kTz__TZE204_lyqazpe6[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
