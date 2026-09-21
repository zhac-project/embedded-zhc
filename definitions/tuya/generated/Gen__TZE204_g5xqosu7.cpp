// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_g5xqosu7).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_g5xqosu7_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_g5xqosu7_reverse_direction_dp5[] = {
    { 0, "forward" },
    { 1, "back" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_g5xqosu7_motor_working_mode_dp106[] = {
    { 0, "continuous" },
    { 1, "intermittently" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_g5xqosu7[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_g5xqosu7_state_dp1, sizeof(kEnum__TZE204_g5xqosu7_state_dp1)/sizeof(kEnum__TZE204_g5xqosu7_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 5, "reverse_direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_g5xqosu7_reverse_direction_dp5, sizeof(kEnum__TZE204_g5xqosu7_reverse_direction_dp5)/sizeof(kEnum__TZE204_g5xqosu7_reverse_direction_dp5[0]) },
    { 12, "motor_fault", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 106, "motor_working_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_g5xqosu7_motor_working_mode_dp106, sizeof(kEnum__TZE204_g5xqosu7_motor_working_mode_dp106)/sizeof(kEnum__TZE204_g5xqosu7_motor_working_mode_dp106[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_g5xqosu7{ kEntries__TZE204_g5xqosu7, 6 };
constexpr FzConverter kFzDp__TZE204_g5xqosu7{
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
    .user_config       = &kMap__TZE204_g5xqosu7,
};
const FzConverter* const kFz__TZE204_g5xqosu7[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_g5xqosu7,
};
constexpr TzConverter kTzDp__TZE204_g5xqosu7{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_g5xqosu7,
};
const TzConverter* const kTz__TZE204_g5xqosu7[] = { &kTzDp__TZE204_g5xqosu7 };
constexpr const char* kM__TZE204_g5xqosu7[] = { "TS0601" };
constexpr const char* kN__TZE204_g5xqosu7[] = { "_TZE204_g5xqosu7" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE204_g5xqosu7_0[] = {"OPEN", "CLOSE", "STOP"};
constexpr const char* kAutoOpts__TZE204_g5xqosu7_2[] = {"forward", "back"};
constexpr const char* kAutoOpts__TZE204_g5xqosu7_4[] = {"SET", "RESET"};
constexpr const char* kAutoOpts__TZE204_g5xqosu7_5[] = {"SET", "RESET"};
constexpr const char* kAutoOpts__TZE204_g5xqosu7_6[] = {"SET", "RESET"};
constexpr const char* kAutoOpts__TZE204_g5xqosu7_7[] = {"continuous", "intermittently"};
constexpr Expose kAutoExposes__TZE204_g5xqosu7[] = {
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_g5xqosu7_0, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"reverse_direction", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_g5xqosu7_2, 2},
    {"motor_fault", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"upper_stroke_limit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_g5xqosu7_4, 2, ExposeCategory::Config},
    {"middle_stroke_limit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_g5xqosu7_5, 2, ExposeCategory::Config},
    {"lower_stroke_limit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_g5xqosu7_6, 2, ExposeCategory::Config},
    {"motor_working_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_g5xqosu7_7, 2, ExposeCategory::Config},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_g5xqosu7[] = {
    {"Lilistore","TS0601_lilistore"},
    {"Zemismart","ZM90E-DT250N/A400"},
    {"Nova Digital","CMR-1"},
};
extern const PreparedDefinition kDefGen__TZE204_g5xqosu7{
    .zigbee_models=kM__TZE204_g5xqosu7,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_g5xqosu7,.manufacturer_names_count=1,
    .model="TS0601__TZE204_g5xqosu7",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_g5xqosu7,.exposes_count=sizeof(kAutoExposes__TZE204_g5xqosu7)/sizeof(kAutoExposes__TZE204_g5xqosu7[0]),
    .white_labels=kWhiteLabels_Gen_TZE204_g5xqosu7, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_g5xqosu7)/sizeof(kWhiteLabels_Gen_TZE204_g5xqosu7[0]),
    .from_zigbee=kFz__TZE204_g5xqosu7,
    .from_zigbee_count=sizeof(kFz__TZE204_g5xqosu7)/sizeof(kFz__TZE204_g5xqosu7[0]),
    .to_zigbee=kTz__TZE204_g5xqosu7,
    .to_zigbee_count=sizeof(kTz__TZE204_g5xqosu7)/sizeof(kTz__TZE204_g5xqosu7[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
