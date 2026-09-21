// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_eevqq1uv).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_eevqq1uv_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_eevqq1uv_reverse_direction_dp5[] = {
    { 0, "forward" },
    { 1, "back" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_eevqq1uv_border_dp16[] = {
    { 0, "up" },
    { 1, "down" },
    { 2, "up_delete" },
    { 3, "down_delete" },
    { 4, "remove_top_bottom" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_eevqq1uv_click_control_dp20[] = {
    { 0, "up" },
    { 1, "down" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_eevqq1uv[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_eevqq1uv_state_dp1, sizeof(kEnum__TZE200_eevqq1uv_state_dp1)/sizeof(kEnum__TZE200_eevqq1uv_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "reverse_direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_eevqq1uv_reverse_direction_dp5, sizeof(kEnum__TZE200_eevqq1uv_reverse_direction_dp5)/sizeof(kEnum__TZE200_eevqq1uv_reverse_direction_dp5[0]) },
    { 12, "motor_fault", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 13, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "border", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_eevqq1uv_border_dp16, sizeof(kEnum__TZE200_eevqq1uv_border_dp16)/sizeof(kEnum__TZE200_eevqq1uv_border_dp16[0]) },
    { 20, "click_control", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_eevqq1uv_click_control_dp20, sizeof(kEnum__TZE200_eevqq1uv_click_control_dp20)/sizeof(kEnum__TZE200_eevqq1uv_click_control_dp20[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_eevqq1uv{ kEntries__TZE200_eevqq1uv, 8 };
constexpr FzConverter kFzDp__TZE200_eevqq1uv{
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
    .user_config       = &kMap__TZE200_eevqq1uv,
};
const FzConverter* const kFz__TZE200_eevqq1uv[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_eevqq1uv,
};
constexpr TzConverter kTzDp__TZE200_eevqq1uv{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_eevqq1uv,
};
const TzConverter* const kTz__TZE200_eevqq1uv[] = { &kTzDp__TZE200_eevqq1uv };
constexpr const char* kM__TZE200_eevqq1uv[] = { "TS0601" };
constexpr const char* kN__TZE200_eevqq1uv[] = { "_TZE200_eevqq1uv" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_eevqq1uv_1[] = {"OPEN", "CLOSE", "STOP"};
constexpr const char* kAutoOpts__TZE200_eevqq1uv_3[] = {"forward", "back"};
constexpr const char* kAutoOpts__TZE200_eevqq1uv_4[] = {"set_up", "set_down", "delete_up", "delete_down", "delete_both"};
constexpr const char* kAutoOpts__TZE200_eevqq1uv_5[] = {"up", "down"};
constexpr Expose kAutoExposes__TZE200_eevqq1uv[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_eevqq1uv_1, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"reverse_direction", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_eevqq1uv_3, 2},
    {"cover_limit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_eevqq1uv_4, 5, ExposeCategory::Config},
    {"click_control", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_eevqq1uv_5, 2},
    {"motor_fault", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_eevqq1uv[] = {
    {"Zemismart","ZM16EL-03/33"},
    {"Zemismart","ZM25EL"},
    {"Zemismart","ZM85EL-2Z"},
    {"Hiladuo","B09M3R35GC"},
    {"Tuya","MYQ-RM25-1.3/25-BZ"},
    {"Shaman","25EB-1/30-TYZ"},
};
extern const PreparedDefinition kDefGen__TZE200_eevqq1uv{
    .zigbee_models=kM__TZE200_eevqq1uv,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_eevqq1uv,.manufacturer_names_count=1,
    .model="TS0601__TZE200_eevqq1uv",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_eevqq1uv,.exposes_count=sizeof(kAutoExposes__TZE200_eevqq1uv)/sizeof(kAutoExposes__TZE200_eevqq1uv[0]),
    .white_labels=kWhiteLabels_Gen_TZE200_eevqq1uv, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_eevqq1uv)/sizeof(kWhiteLabels_Gen_TZE200_eevqq1uv[0]),
    .from_zigbee=kFz__TZE200_eevqq1uv,
    .from_zigbee_count=sizeof(kFz__TZE200_eevqq1uv)/sizeof(kFz__TZE200_eevqq1uv[0]),
    .to_zigbee=kTz__TZE200_eevqq1uv,
    .to_zigbee_count=sizeof(kTz__TZE200_eevqq1uv)/sizeof(kTz__TZE200_eevqq1uv[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
