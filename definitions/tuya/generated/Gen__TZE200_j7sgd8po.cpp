// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_j7sgd8po).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_j7sgd8po_alarm_dp102[] = {
    { 0, "IDLE" },
    { 1, "ALARM" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_j7sgd8po_opening_mode_dp103[] = {
    { 0, "closed" },
    { 1, "tilted" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_j7sgd8po_position_dp104[] = {
    { 4, "left" },
    { 1, "up" },
    { 2, "down" },
    { 3, "right" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_j7sgd8po_button_left_dp105[] = {
    { 0, "released" },
    { 1, "pressed" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_j7sgd8po_button_right_dp106[] = {
    { 0, "released" },
    { 1, "pressed" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_j7sgd8po_vacation_dp107[] = {
    { 0, "OFF" },
    { 1, "ON" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_j7sgd8po_sensitivity_dp108[] = {
    { 0, "off" },
    { 1, "low" },
    { 2, "medium" },
    { 3, "high" },
    { 4, "max" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_j7sgd8po_alarm_switch_dp109[] = {
    { 0, "OFF" },
    { 1, "ON" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_j7sgd8po_keysound_dp111[] = {
    { 0, "OFF" },
    { 1, "ON" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_j7sgd8po_battery_low_dp112[] = {
    { 0, "ON" },
    { 1, "OFF" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_j7sgd8po_handlesound_dp114[] = {
    { 0, "OFF" },
    { 1, "ON" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_j7sgd8po_calibrate_dp120[] = {
    { 0, "clear" },
    { 1, "execute" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_j7sgd8po[] = {
    { 3, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 101, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "alarm", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_j7sgd8po_alarm_dp102, sizeof(kEnum__TZE200_j7sgd8po_alarm_dp102)/sizeof(kEnum__TZE200_j7sgd8po_alarm_dp102[0]) },
    { 103, "opening_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_j7sgd8po_opening_mode_dp103, sizeof(kEnum__TZE200_j7sgd8po_opening_mode_dp103)/sizeof(kEnum__TZE200_j7sgd8po_opening_mode_dp103[0]) },
    { 104, "position", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_j7sgd8po_position_dp104, sizeof(kEnum__TZE200_j7sgd8po_position_dp104)/sizeof(kEnum__TZE200_j7sgd8po_position_dp104[0]) },
    { 105, "button_left", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_j7sgd8po_button_left_dp105, sizeof(kEnum__TZE200_j7sgd8po_button_left_dp105)/sizeof(kEnum__TZE200_j7sgd8po_button_left_dp105[0]) },
    { 106, "button_right", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_j7sgd8po_button_right_dp106, sizeof(kEnum__TZE200_j7sgd8po_button_right_dp106)/sizeof(kEnum__TZE200_j7sgd8po_button_right_dp106[0]) },
    { 107, "vacation", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_j7sgd8po_vacation_dp107, sizeof(kEnum__TZE200_j7sgd8po_vacation_dp107)/sizeof(kEnum__TZE200_j7sgd8po_vacation_dp107[0]) },
    { 108, "sensitivity", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_j7sgd8po_sensitivity_dp108, sizeof(kEnum__TZE200_j7sgd8po_sensitivity_dp108)/sizeof(kEnum__TZE200_j7sgd8po_sensitivity_dp108[0]) },
    { 109, "alarm_switch", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_j7sgd8po_alarm_switch_dp109, sizeof(kEnum__TZE200_j7sgd8po_alarm_switch_dp109)/sizeof(kEnum__TZE200_j7sgd8po_alarm_switch_dp109[0]) },
    { 110, "update_frequency", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "keysound", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_j7sgd8po_keysound_dp111, sizeof(kEnum__TZE200_j7sgd8po_keysound_dp111)/sizeof(kEnum__TZE200_j7sgd8po_keysound_dp111[0]) },
    { 112, "battery_low", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_j7sgd8po_battery_low_dp112, sizeof(kEnum__TZE200_j7sgd8po_battery_low_dp112)/sizeof(kEnum__TZE200_j7sgd8po_battery_low_dp112[0]) },
    { 113, "duration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 114, "handlesound", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_j7sgd8po_handlesound_dp114, sizeof(kEnum__TZE200_j7sgd8po_handlesound_dp114)/sizeof(kEnum__TZE200_j7sgd8po_handlesound_dp114[0]) },
    { 120, "calibrate", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_j7sgd8po_calibrate_dp120, sizeof(kEnum__TZE200_j7sgd8po_calibrate_dp120)/sizeof(kEnum__TZE200_j7sgd8po_calibrate_dp120[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_j7sgd8po{ kEntries__TZE200_j7sgd8po, 17 };
constexpr FzConverter kFzDp__TZE200_j7sgd8po{
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
    .user_config       = &kMap__TZE200_j7sgd8po,
};
const FzConverter* const kFz__TZE200_j7sgd8po[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_j7sgd8po,
};
constexpr TzConverter kTzDp__TZE200_j7sgd8po{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_j7sgd8po,
};
const TzConverter* const kTz__TZE200_j7sgd8po[] = { &kTzDp__TZE200_j7sgd8po };
constexpr const char* kM__TZE200_j7sgd8po[] = { "TS0601" };
constexpr const char* kN__TZE200_j7sgd8po[] = { "_TZE200_j7sgd8po" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_j7sgd8po_3[] = {"ALARM", "IDLE"};
constexpr const char* kAutoOpts__TZE200_j7sgd8po_6[] = {"closed", "tilted"};
constexpr const char* kAutoOpts__TZE200_j7sgd8po_10[] = {"off", "low", "medium", "high", "max"};
constexpr const char* kAutoOpts__TZE200_j7sgd8po_11[] = {"up", "right", "down", "left"};
constexpr const char* kAutoOpts__TZE200_j7sgd8po_12[] = {"released", "pressed"};
constexpr const char* kAutoOpts__TZE200_j7sgd8po_13[] = {"released", "pressed"};
constexpr const char* kAutoOpts__TZE200_j7sgd8po_16[] = {"clear", "execute"};
constexpr Expose kAutoExposes__TZE200_j7sgd8po[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Diagnostic},
    {"vacation", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"alarm", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_j7sgd8po_3, 2},
    {"alarm_switch", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"handlesound", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"opening_mode", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_j7sgd8po_6, 2},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"keysound", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"sensitivity", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_j7sgd8po_10, 5},
    {"position", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_j7sgd8po_11, 4},
    {"button_left", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_j7sgd8po_12, 2},
    {"button_right", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_j7sgd8po_13, 2},
    {"duration", ExposeType::Numeric, Access::StateSet, "sec", nullptr, nullptr, 0, ExposeCategory::State, 0, 300, 1},
    {"update_frequency", ExposeType::Numeric, Access::StateSet, "min", nullptr, nullptr, 0, ExposeCategory::State, 0, 700, 0},
    {"calibrate", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_j7sgd8po_16, 2},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_j7sgd8po{
    .zigbee_models=kM__TZE200_j7sgd8po,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_j7sgd8po,.manufacturer_names_count=1,
    .model="TS0601__TZE200_j7sgd8po",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_j7sgd8po,.exposes_count=sizeof(kAutoExposes__TZE200_j7sgd8po)/sizeof(kAutoExposes__TZE200_j7sgd8po[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_j7sgd8po,
    .from_zigbee_count=sizeof(kFz__TZE200_j7sgd8po)/sizeof(kFz__TZE200_j7sgd8po[0]),
    .to_zigbee=kTz__TZE200_j7sgd8po,
    .to_zigbee_count=sizeof(kTz__TZE200_j7sgd8po)/sizeof(kTz__TZE200_j7sgd8po[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
