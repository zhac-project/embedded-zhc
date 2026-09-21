// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_eqpaxqdv).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_eqpaxqdv_state_dp1[] = {
    { 0, "open" },
    { 1, "stop" },
    { 2, "close" },
    { 3, "lock" },
    { 4, "unlock" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_eqpaxqdv_mode_dp4[] = {
    { 0, "up" },
    { 1, "up_delete" },
    { 2, "remove_up_down" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_eqpaxqdv_control_back_dp5[] = {
    { 0, "forward" },
    { 1, "back" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_eqpaxqdv_work_state_dp7[] = {
    { 0, "opening" },
    { 1, "closing" },
    { 2, "123" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_eqpaxqdv_situation_set_dp11[] = {
    { 0, "fully_open" },
    { 1, "fully_close" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_eqpaxqdv_border_dp16[] = {
    { 0, "down_delete" },
    { 1, "remove_top_bottom" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_eqpaxqdv_calibration_dp101[] = {
    { 0, "start" },
    { 1, "end" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_eqpaxqdv_reset_dp104[] = {
    { 0, "reset" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_eqpaxqdv[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_eqpaxqdv_state_dp1, sizeof(kEnum__TZE200_eqpaxqdv_state_dp1)/sizeof(kEnum__TZE200_eqpaxqdv_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 4, "mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_eqpaxqdv_mode_dp4, sizeof(kEnum__TZE200_eqpaxqdv_mode_dp4)/sizeof(kEnum__TZE200_eqpaxqdv_mode_dp4[0]) },
    { 5, "control_back", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_eqpaxqdv_control_back_dp5, sizeof(kEnum__TZE200_eqpaxqdv_control_back_dp5)/sizeof(kEnum__TZE200_eqpaxqdv_control_back_dp5[0]) },
    { 6, "auto_power", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "work_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_eqpaxqdv_work_state_dp7, sizeof(kEnum__TZE200_eqpaxqdv_work_state_dp7)/sizeof(kEnum__TZE200_eqpaxqdv_work_state_dp7[0]) },
    { 10, "time_total", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 11, "situation_set", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_eqpaxqdv_situation_set_dp11, sizeof(kEnum__TZE200_eqpaxqdv_situation_set_dp11)/sizeof(kEnum__TZE200_eqpaxqdv_situation_set_dp11[0]) },
    { 12, "fault", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "border", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_eqpaxqdv_border_dp16, sizeof(kEnum__TZE200_eqpaxqdv_border_dp16)/sizeof(kEnum__TZE200_eqpaxqdv_border_dp16[0]) },
    { 19, "position_best", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 21, "angle_horizontal", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "calibration", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_eqpaxqdv_calibration_dp101, sizeof(kEnum__TZE200_eqpaxqdv_calibration_dp101)/sizeof(kEnum__TZE200_eqpaxqdv_calibration_dp101[0]) },
    { 102, "quick_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "switch", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "reset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_eqpaxqdv_reset_dp104, sizeof(kEnum__TZE200_eqpaxqdv_reset_dp104)/sizeof(kEnum__TZE200_eqpaxqdv_reset_dp104[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_eqpaxqdv{ kEntries__TZE200_eqpaxqdv, 17 };
constexpr FzConverter kFzDp__TZE200_eqpaxqdv{
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
    .user_config       = &kMap__TZE200_eqpaxqdv,
};
const FzConverter* const kFz__TZE200_eqpaxqdv[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_eqpaxqdv,
};
constexpr TzConverter kTzDp__TZE200_eqpaxqdv{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_eqpaxqdv,
};
const TzConverter* const kTz__TZE200_eqpaxqdv[] = { &kTzDp__TZE200_eqpaxqdv };
constexpr const char* kM__TZE200_eqpaxqdv[] = { "TS0601" };
constexpr const char* kN__TZE200_eqpaxqdv[] = { "_TZE200_eqpaxqdv" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_eqpaxqdv_0[] = {"open", "stop", "close", "lock", "unlock"};
constexpr const char* kAutoOpts__TZE200_eqpaxqdv_2[] = {"up", "up_delete", "remove_up_down"};
constexpr const char* kAutoOpts__TZE200_eqpaxqdv_3[] = {"forward", "back"};
constexpr const char* kAutoOpts__TZE200_eqpaxqdv_5[] = {"opening", "closing", "123"};
constexpr const char* kAutoOpts__TZE200_eqpaxqdv_7[] = {"fully_open", "fully_close"};
constexpr const char* kAutoOpts__TZE200_eqpaxqdv_9[] = {"set_up", "set_down", "delete_up", "delete_down", "delete_both"};
constexpr const char* kAutoOpts__TZE200_eqpaxqdv_12[] = {"start", "end"};
constexpr const char* kAutoOpts__TZE200_eqpaxqdv_15[] = {"reset"};
constexpr Expose kAutoExposes__TZE200_eqpaxqdv[] = {
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_eqpaxqdv_0, 5},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_eqpaxqdv_2, 3},
    {"control_back", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_eqpaxqdv_3, 2},
    {"auto_power", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"work_state", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_eqpaxqdv_5, 3},
    {"time_total", ExposeType::Numeric, Access::State, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 600, 0},
    {"situation_set", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_eqpaxqdv_7, 2},
    {"fault", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"cover_limit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_eqpaxqdv_9, 5, ExposeCategory::Config},
    {"favorite_position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::Config, 0, 100, 1},
    {"angle_horizontal", ExposeType::Numeric, Access::StateSet, "°", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 25},
    {"calibration", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_eqpaxqdv_12, 2},
    {"quick_calibration", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 900, 0},
    {"switch", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"reset", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_eqpaxqdv_15, 1},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_eqpaxqdv{
    .zigbee_models=kM__TZE200_eqpaxqdv,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_eqpaxqdv,.manufacturer_names_count=1,
    .model="TS0601__TZE200_eqpaxqdv",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_eqpaxqdv,.exposes_count=sizeof(kAutoExposes__TZE200_eqpaxqdv)/sizeof(kAutoExposes__TZE200_eqpaxqdv[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_eqpaxqdv,
    .from_zigbee_count=sizeof(kFz__TZE200_eqpaxqdv)/sizeof(kFz__TZE200_eqpaxqdv[0]),
    .to_zigbee=kTz__TZE200_eqpaxqdv,
    .to_zigbee_count=sizeof(kTz__TZE200_eqpaxqdv)/sizeof(kTz__TZE200_eqpaxqdv[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
