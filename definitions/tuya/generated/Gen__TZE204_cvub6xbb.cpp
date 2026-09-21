// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_cvub6xbb).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_cvub6xbb_preset_dp4[] = {
    { 0, "manual" },
    { 1, "auto" },
    { 3, "eco" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_cvub6xbb_running_state_dp101[] = {
    { 1, "heat" },
    { 0, "idle" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_cvub6xbb_sensor_dp106[] = {
    { 0, "internal" },
    { 1, "external" },
    { 2, "both" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_cvub6xbb_backlight_mode_dp110[] = {
    { 0, "off" },
    { 1, "always_low" },
    { 2, "always_mid" },
    { 3, "always_high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_cvub6xbb[] = {
    { 2, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 3, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 4, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_cvub6xbb_preset_dp4, sizeof(kEnum__TZE204_cvub6xbb_preset_dp4)/sizeof(kEnum__TZE204_cvub6xbb_preset_dp4[0]) },
    { 9, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 15, "max_temperature_limit", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 19, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_cvub6xbb_running_state_dp101, sizeof(kEnum__TZE204_cvub6xbb_running_state_dp101)/sizeof(kEnum__TZE204_cvub6xbb_running_state_dp101[0]) },
    { 102, "frost_protection", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 103, "factory_reset", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 106, "sensor", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_cvub6xbb_sensor_dp106, sizeof(kEnum__TZE204_cvub6xbb_sensor_dp106)/sizeof(kEnum__TZE204_cvub6xbb_sensor_dp106[0]) },
    { 107, "temperature_delta", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 110, "backlight_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_cvub6xbb_backlight_mode_dp110, sizeof(kEnum__TZE204_cvub6xbb_backlight_mode_dp110)/sizeof(kEnum__TZE204_cvub6xbb_backlight_mode_dp110[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_cvub6xbb{ kEntries__TZE204_cvub6xbb, 12 };
constexpr FzConverter kFzDp__TZE204_cvub6xbb{
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
    .user_config       = &kMap__TZE204_cvub6xbb,
};
const FzConverter* const kFz__TZE204_cvub6xbb[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_cvub6xbb,
};
constexpr TzConverter kTzDp__TZE204_cvub6xbb{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_cvub6xbb,
};
const TzConverter* const kTz__TZE204_cvub6xbb[] = { &kTzDp__TZE204_cvub6xbb };
constexpr const char* kM__TZE204_cvub6xbb[] = { "TS0601" };
constexpr const char* kN__TZE204_cvub6xbb[] = { "_TZE204_cvub6xbb" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE204_cvub6xbb_1[] = {"internal", "external", "both"};
constexpr const char* kAutoOpts__TZE204_cvub6xbb_2[] = {"off", "heat"};
constexpr const char* kAutoOpts__TZE204_cvub6xbb_3[] = {"manual", "auto", "eco"};
constexpr const char* kAutoOpts__TZE204_cvub6xbb_5[] = {"idle", "heat"};
constexpr const char* kAutoOpts__TZE204_cvub6xbb_8[] = {"off", "always_low", "always_mid", "always_high"};
constexpr Expose kAutoExposes__TZE204_cvub6xbb[] = {
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"sensor", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_cvub6xbb_1, 3, ExposeCategory::Config},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_cvub6xbb_2, 2},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_cvub6xbb_3, 3},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"running_state", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_cvub6xbb_5, 2},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -9, 9, 0},
    {"backlight_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_cvub6xbb_8, 4},
    {"frost_protection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"max_temperature_limit", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 15, 90, 0},
    {"temperature_delta", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 0, 10, 0},
    {"factory_reset", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_cvub6xbb{
    .zigbee_models=kM__TZE204_cvub6xbb,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_cvub6xbb,.manufacturer_names_count=1,
    .model="TS0601__TZE204_cvub6xbb",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_cvub6xbb,.exposes_count=sizeof(kAutoExposes__TZE204_cvub6xbb)/sizeof(kAutoExposes__TZE204_cvub6xbb[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_cvub6xbb,
    .from_zigbee_count=sizeof(kFz__TZE204_cvub6xbb)/sizeof(kFz__TZE204_cvub6xbb[0]),
    .to_zigbee=kTz__TZE204_cvub6xbb,
    .to_zigbee_count=sizeof(kTz__TZE204_cvub6xbb)/sizeof(kTz__TZE204_cvub6xbb[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
