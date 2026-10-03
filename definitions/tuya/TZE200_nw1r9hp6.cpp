// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: _TZE200_nw1r9hp6 curtain motor, Zemismart ZM85EL-2Z (z2m TS0601_cover_3).
//
// z2m has listed this fingerprint as TS0601_cover_3 since 2023-06-03
// (zigbee2mqtt#11251). The first port here called it a smoke detector, so the
// motor's DP1 command published `smoke` and moving the curtain looked like a
// fire. Same datapoints as its generated siblings (Gen__TZE200_pw7mji0l & co).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nw1r9hp6_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nw1r9hp6_reverse_direction_dp5[] = {
    { 0, "forward" },
    { 1, "back" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nw1r9hp6_border_dp16[] = {
    { 0, "up" },
    { 1, "down" },
    { 2, "up_delete" },
    { 3, "down_delete" },
    { 4, "remove_top_bottom" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_nw1r9hp6_click_control_dp20[] = {
    { 0, "up" },
    { 1, "down" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_nw1r9hp6[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nw1r9hp6_state_dp1, sizeof(kEnum__TZE200_nw1r9hp6_state_dp1)/sizeof(kEnum__TZE200_nw1r9hp6_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "reverse_direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nw1r9hp6_reverse_direction_dp5, sizeof(kEnum__TZE200_nw1r9hp6_reverse_direction_dp5)/sizeof(kEnum__TZE200_nw1r9hp6_reverse_direction_dp5[0]) },
    { 12, "motor_fault", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 13, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "border", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nw1r9hp6_border_dp16, sizeof(kEnum__TZE200_nw1r9hp6_border_dp16)/sizeof(kEnum__TZE200_nw1r9hp6_border_dp16[0]) },
    { 20, "click_control", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_nw1r9hp6_click_control_dp20, sizeof(kEnum__TZE200_nw1r9hp6_click_control_dp20)/sizeof(kEnum__TZE200_nw1r9hp6_click_control_dp20[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_nw1r9hp6{ kEntries__TZE200_nw1r9hp6, 8 };
constexpr FzConverter kFzDp__TZE200_nw1r9hp6{
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
    .user_config       = &kMap__TZE200_nw1r9hp6,
};
const FzConverter* const kFz__TZE200_nw1r9hp6[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_nw1r9hp6,
};
constexpr TzConverter kTzDp__TZE200_nw1r9hp6{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_nw1r9hp6,
};
const TzConverter* const kTz__TZE200_nw1r9hp6[] = { &kTzDp__TZE200_nw1r9hp6 };
constexpr const char* kM__TZE200_nw1r9hp6[] = { "TS0601" };
constexpr const char* kN__TZE200_nw1r9hp6[] = { "_TZE200_nw1r9hp6" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_nw1r9hp6_1[] = {"OPEN", "CLOSE", "STOP"};
constexpr const char* kAutoOpts__TZE200_nw1r9hp6_3[] = {"forward", "back"};
constexpr const char* kAutoOpts__TZE200_nw1r9hp6_4[] = {"set_up", "set_down", "delete_up", "delete_down", "delete_both"};
constexpr const char* kAutoOpts__TZE200_nw1r9hp6_5[] = {"up", "down"};
constexpr Expose kAutoExposes__TZE200_nw1r9hp6[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_nw1r9hp6_1, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"reverse_direction", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_nw1r9hp6_3, 2},
    {"cover_limit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_nw1r9hp6_4, 5, ExposeCategory::Config},
    {"click_control", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_nw1r9hp6_5, 2},
    {"motor_fault", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_TZE200_nw1r9hp6[] = {
    {"Zemismart","ZM85EL-2Z"},
};
extern const PreparedDefinition kDefTZE200_nw1r9hp6{
    .zigbee_models=kM__TZE200_nw1r9hp6,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_nw1r9hp6,.manufacturer_names_count=1,
    .model="TS0601_cover_3",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_nw1r9hp6,.exposes_count=sizeof(kAutoExposes__TZE200_nw1r9hp6)/sizeof(kAutoExposes__TZE200_nw1r9hp6[0]),
    .white_labels=kWhiteLabels_TZE200_nw1r9hp6, .white_labels_count=sizeof(kWhiteLabels_TZE200_nw1r9hp6)/sizeof(kWhiteLabels_TZE200_nw1r9hp6[0]),
    .from_zigbee=kFz__TZE200_nw1r9hp6,
    .from_zigbee_count=sizeof(kFz__TZE200_nw1r9hp6)/sizeof(kFz__TZE200_nw1r9hp6[0]),
    .to_zigbee=kTz__TZE200_nw1r9hp6,
    .to_zigbee_count=sizeof(kTz__TZE200_nw1r9hp6)/sizeof(kTz__TZE200_nw1r9hp6[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
