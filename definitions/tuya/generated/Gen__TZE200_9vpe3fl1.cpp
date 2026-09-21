// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_9vpe3fl1).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_9vpe3fl1_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_9vpe3fl1_reverse_direction_dp5[] = {
    { 0, "forward" },
    { 1, "back" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_9vpe3fl1_motor_working_mode_dp106[] = {
    { 0, "continuous" },
    { 1, "intermittently" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_9vpe3fl1[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_9vpe3fl1_state_dp1, sizeof(kEnum__TZE200_9vpe3fl1_state_dp1)/sizeof(kEnum__TZE200_9vpe3fl1_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition },
    { 5, "reverse_direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_9vpe3fl1_reverse_direction_dp5, sizeof(kEnum__TZE200_9vpe3fl1_reverse_direction_dp5)/sizeof(kEnum__TZE200_9vpe3fl1_reverse_direction_dp5[0]) },
    { 12, "motor_fault", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 106, "motor_working_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_9vpe3fl1_motor_working_mode_dp106, sizeof(kEnum__TZE200_9vpe3fl1_motor_working_mode_dp106)/sizeof(kEnum__TZE200_9vpe3fl1_motor_working_mode_dp106[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_9vpe3fl1{ kEntries__TZE200_9vpe3fl1, 6 };
constexpr FzConverter kFzDp__TZE200_9vpe3fl1{
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
    .user_config       = &kMap__TZE200_9vpe3fl1,
};
const FzConverter* const kFz__TZE200_9vpe3fl1[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_9vpe3fl1,
};
constexpr TzConverter kTzDp__TZE200_9vpe3fl1{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_9vpe3fl1,
};
const TzConverter* const kTz__TZE200_9vpe3fl1[] = { &kTzDp__TZE200_9vpe3fl1 };
constexpr const char* kM__TZE200_9vpe3fl1[] = { "TS0601" };
constexpr const char* kN__TZE200_9vpe3fl1[] = { "_TZE200_9vpe3fl1" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_9vpe3fl1_0[] = {"OPEN", "CLOSE", "STOP"};
constexpr const char* kAutoOpts__TZE200_9vpe3fl1_2[] = {"forward", "back"};
constexpr const char* kAutoOpts__TZE200_9vpe3fl1_4[] = {"SET", "RESET"};
constexpr const char* kAutoOpts__TZE200_9vpe3fl1_5[] = {"SET", "RESET"};
constexpr const char* kAutoOpts__TZE200_9vpe3fl1_6[] = {"SET", "RESET"};
constexpr const char* kAutoOpts__TZE200_9vpe3fl1_7[] = {"continuous", "intermittently"};
constexpr Expose kAutoExposes__TZE200_9vpe3fl1[] = {
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_9vpe3fl1_0, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"reverse_direction", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_9vpe3fl1_2, 2},
    {"motor_fault", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"upper_stroke_limit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_9vpe3fl1_4, 2, ExposeCategory::Config},
    {"middle_stroke_limit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_9vpe3fl1_5, 2, ExposeCategory::Config},
    {"lower_stroke_limit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_9vpe3fl1_6, 2, ExposeCategory::Config},
    {"motor_working_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_9vpe3fl1_7, 2, ExposeCategory::Config},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_9vpe3fl1[] = {
    {"Lilistore","TS0601_lilistore"},
    {"Zemismart","ZM90E-DT250N/A400"},
    {"Nova Digital","CMR-1"},
};
extern const PreparedDefinition kDefGen__TZE200_9vpe3fl1{
    .zigbee_models=kM__TZE200_9vpe3fl1,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_9vpe3fl1,.manufacturer_names_count=1,
    .model="TS0601__TZE200_9vpe3fl1",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_9vpe3fl1,.exposes_count=sizeof(kAutoExposes__TZE200_9vpe3fl1)/sizeof(kAutoExposes__TZE200_9vpe3fl1[0]),
    .white_labels=kWhiteLabels_Gen_TZE200_9vpe3fl1, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_9vpe3fl1)/sizeof(kWhiteLabels_Gen_TZE200_9vpe3fl1[0]),
    .from_zigbee=kFz__TZE200_9vpe3fl1,
    .from_zigbee_count=sizeof(kFz__TZE200_9vpe3fl1)/sizeof(kFz__TZE200_9vpe3fl1[0]),
    .to_zigbee=kTz__TZE200_9vpe3fl1,
    .to_zigbee_count=sizeof(kTz__TZE200_9vpe3fl1)/sizeof(kTz__TZE200_9vpe3fl1[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
