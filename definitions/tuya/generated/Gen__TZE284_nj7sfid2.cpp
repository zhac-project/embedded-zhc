// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_nj7sfid2).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_nj7sfid2[] = {
    { 1, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 2, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 52, "binding_confirmation", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "binding_config", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_nj7sfid2{ kEntries__TZE284_nj7sfid2, 6 };
constexpr FzConverter kFzDp__TZE284_nj7sfid2{
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
    .user_config       = &kMap__TZE284_nj7sfid2,
};
const FzConverter* const kFz__TZE284_nj7sfid2[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_nj7sfid2,
};
constexpr TzConverter kTzDp__TZE284_nj7sfid2{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_nj7sfid2,
};
const TzConverter* const kTz__TZE284_nj7sfid2[] = { &kTzDp__TZE284_nj7sfid2 };
constexpr const char* kM__TZE284_nj7sfid2[] = { "TS0601" };
constexpr const char* kN__TZE284_nj7sfid2[] = { "_TZE284_nj7sfid2" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE284_nj7sfid2_3[] = {"unassigned", "waiting_button_1", "ready"};
constexpr const char* kAutoOpts__TZE284_nj7sfid2_7[] = {"bind"};
constexpr const char* kAutoOpts__TZE284_nj7sfid2_8[] = {"bind"};
constexpr const char* kAutoOpts__TZE284_nj7sfid2_9[] = {"bind"};
constexpr const char* kAutoOpts__TZE284_nj7sfid2_10[] = {"assign"};
constexpr const char* kAutoOpts__TZE284_nj7sfid2_12[] = {"scene_1", "scene_2", "scene_3", "scene_4", "light_1_on", "light_1_off", "light_1_brightness_up", "light_1_brightness_down", "light_1_colortemp_up", "light_1_colortemp_down", "light_2_on", "light_2_off", "light_2_brightness_up", "light_2_brightness_down", "light_2_colortemp_up", "light_2_colortemp_down", "light_3_on", "light_3_off", "light_3_brightness_up", "light_3_brightness_down", "light_3_colortemp_up", "light_3_colortemp_down", "light_4_on", "light_4_off", "light_4_brightness_up", "light_4_brightness_down", "light_4_colortemp_up", "light_4_colortemp_down", "curtain_1_start", "curtain_1_stop", "curtain_1_position_open", "curtain_1_position_close", "curtain_2_start", "curtain_2_stop", "curtain_2_position_open", "curtain_2_position_close", "curtain_3_start", "curtain_3_stop", "curtain_3_position_open", "curtain_3_position_close", "curtain_4_start", "curtain_4_stop", "curtain_4_position_open", "curtain_4_position_close"};
constexpr Expose kAutoExposes__TZE284_nj7sfid2[] = {
    {"brightness", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 255, 0},
    {"color_temp", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 153, 523, 0},
    {"curtain_position", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"assignment_status", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE284_nj7sfid2_3, 3, ExposeCategory::Diagnostic},
    {"base_group_id", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Diagnostic},
    {"action_button", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Diagnostic, 1, 4, 0},
    {"action_group", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Diagnostic},
    {"bind_all_scene", ExposeType::Enum, Access::Set, nullptr, nullptr, kAutoOpts__TZE284_nj7sfid2_7, 1, ExposeCategory::Config},
    {"bind_all_light", ExposeType::Enum, Access::Set, nullptr, nullptr, kAutoOpts__TZE284_nj7sfid2_8, 1, ExposeCategory::Config},
    {"bind_all_curtain", ExposeType::Enum, Access::Set, nullptr, nullptr, kAutoOpts__TZE284_nj7sfid2_9, 1, ExposeCategory::Config},
    {"assign_button_1", ExposeType::Enum, Access::Set, nullptr, nullptr, kAutoOpts__TZE284_nj7sfid2_10, 1, ExposeCategory::Config},
    {"set_base_group_id", ExposeType::Numeric, Access::Set, nullptr, nullptr, nullptr, 0, ExposeCategory::Config, 1, 65000, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE284_nj7sfid2_12, 44, ExposeCategory::Diagnostic},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_nj7sfid2{
    .zigbee_models=kM__TZE284_nj7sfid2,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_nj7sfid2,.manufacturer_names_count=1,
    .model="TS0601__TZE284_nj7sfid2",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_nj7sfid2,.exposes_count=sizeof(kAutoExposes__TZE284_nj7sfid2)/sizeof(kAutoExposes__TZE284_nj7sfid2[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_nj7sfid2,
    .from_zigbee_count=sizeof(kFz__TZE284_nj7sfid2)/sizeof(kFz__TZE284_nj7sfid2[0]),
    .to_zigbee=kTz__TZE284_nj7sfid2,
    .to_zigbee_count=sizeof(kTz__TZE284_nj7sfid2)/sizeof(kTz__TZE284_nj7sfid2[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
