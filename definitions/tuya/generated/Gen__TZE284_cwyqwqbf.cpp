// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_cwyqwqbf).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_cwyqwqbf_battery_state_dp3[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_cwyqwqbf_temperature_unit_dp9[] = {
    { 0, "celsius" },
    { 1, "fahrenheit" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_cwyqwqbf_temperature_alarm_dp14[] = {
    { 0, "lower_alarm" },
    { 1, "upper_alarm" },
    { 2, "cancel" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_cwyqwqbf_humidity_alarm_dp15[] = {
    { 0, "lower_alarm" },
    { 1, "upper_alarm" },
    { 2, "cancel" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_cwyqwqbf[] = {
    { 1, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 2, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_cwyqwqbf_battery_state_dp3, sizeof(kEnum__TZE284_cwyqwqbf_battery_state_dp3)/sizeof(kEnum__TZE284_cwyqwqbf_battery_state_dp3[0]) },
    { 4, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "temperature_unit", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_cwyqwqbf_temperature_unit_dp9, sizeof(kEnum__TZE284_cwyqwqbf_temperature_unit_dp9)/sizeof(kEnum__TZE284_cwyqwqbf_temperature_unit_dp9[0]) },
    { 10, "max_temperature_alarm", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 11, "min_temperature_alarm", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 12, "max_humidity_alarm", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 13, "min_humidity_alarm", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "temperature_alarm", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_cwyqwqbf_temperature_alarm_dp14, sizeof(kEnum__TZE284_cwyqwqbf_temperature_alarm_dp14)/sizeof(kEnum__TZE284_cwyqwqbf_temperature_alarm_dp14[0]) },
    { 15, "humidity_alarm", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_cwyqwqbf_humidity_alarm_dp15, sizeof(kEnum__TZE284_cwyqwqbf_humidity_alarm_dp15)/sizeof(kEnum__TZE284_cwyqwqbf_humidity_alarm_dp15[0]) },
    { 17, "temperature_periodic_report", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "humidity_periodic_report", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 19, "temperature_sensitivity", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 20, "humidity_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_cwyqwqbf{ kEntries__TZE284_cwyqwqbf, 15 };
constexpr FzConverter kFzDp__TZE284_cwyqwqbf{
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
    .user_config       = &kMap__TZE284_cwyqwqbf,
};
const FzConverter* const kFz__TZE284_cwyqwqbf[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_cwyqwqbf,
};
constexpr TzConverter kTzDp__TZE284_cwyqwqbf{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_cwyqwqbf,
};
const TzConverter* const kTz__TZE284_cwyqwqbf[] = { &kTzDp__TZE284_cwyqwqbf };
constexpr const char* kM__TZE284_cwyqwqbf[] = { "TS0601" };
constexpr const char* kN__TZE284_cwyqwqbf[] = { "_TZE284_cwyqwqbf" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE284_cwyqwqbf_2[] = {"celsius", "fahrenheit"};
constexpr const char* kAutoOpts__TZE284_cwyqwqbf_7[] = {"lower_alarm", "upper_alarm", "cancel"};
constexpr const char* kAutoOpts__TZE284_cwyqwqbf_8[] = {"lower_alarm", "upper_alarm", "cancel"};
constexpr Expose kAutoExposes__TZE284_cwyqwqbf[] = {
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"temperature_unit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_cwyqwqbf_2, 2},
    {"max_temperature_alarm", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -20, 60, 0},
    {"min_temperature_alarm", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -20, 60, 0},
    {"max_humidity_alarm", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"min_humidity_alarm", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"temperature_alarm", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE284_cwyqwqbf_7, 3},
    {"humidity_alarm", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE284_cwyqwqbf_8, 3},
    {"temperature_periodic_report", ExposeType::Numeric, Access::StateSet, "min", nullptr, nullptr, 0, ExposeCategory::State, 1, 120, 0},
    {"humidity_periodic_report", ExposeType::Numeric, Access::StateSet, "min", nullptr, nullptr, 0, ExposeCategory::State, 1, 120, 0},
    {"temperature_sensitivity", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 0, 1, 0},
    {"humidity_sensitivity", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 3, 10, 1},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE284_cwyqwqbf[] = {
    {"ONENUO","TH05Z"},
    {"Tuya","TZE284_cwyqwqbf"},
};
extern const PreparedDefinition kDefGen__TZE284_cwyqwqbf{
    .zigbee_models=kM__TZE284_cwyqwqbf,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_cwyqwqbf,.manufacturer_names_count=1,
    .model="TS0601__TZE284_cwyqwqbf",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_cwyqwqbf,.exposes_count=sizeof(kAutoExposes__TZE284_cwyqwqbf)/sizeof(kAutoExposes__TZE284_cwyqwqbf[0]),
    .white_labels=kWhiteLabels_Gen_TZE284_cwyqwqbf, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE284_cwyqwqbf)/sizeof(kWhiteLabels_Gen_TZE284_cwyqwqbf[0]),
    .from_zigbee=kFz__TZE284_cwyqwqbf,
    .from_zigbee_count=sizeof(kFz__TZE284_cwyqwqbf)/sizeof(kFz__TZE284_cwyqwqbf[0]),
    .to_zigbee=kTz__TZE284_cwyqwqbf,
    .to_zigbee_count=sizeof(kTz__TZE284_cwyqwqbf)/sizeof(kTz__TZE284_cwyqwqbf[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
