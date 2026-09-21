// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_ypprdwsl).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_ypprdwsl[] = {
    { 1, "presence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "radar_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "occupancy", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 103, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "tumble_switch", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "tumble_alarm_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 115, "motion_speed", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 117, "static_dwell_alarm", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 118, "fall_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_ypprdwsl{ kEntries__TZE200_ypprdwsl, 9 };
constexpr FzConverter kFzDp__TZE200_ypprdwsl{
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
    .user_config       = &kMap__TZE200_ypprdwsl,
};
const FzConverter* const kFz__TZE200_ypprdwsl[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_ypprdwsl,
};
constexpr TzConverter kTzDp__TZE200_ypprdwsl{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_ypprdwsl,
};
const TzConverter* const kTz__TZE200_ypprdwsl[] = { &kTzDp__TZE200_ypprdwsl };
constexpr const char* kM__TZE200_ypprdwsl[] = { "TS0601" };
constexpr const char* kN__TZE200_ypprdwsl[] = { "_TZE200_ypprdwsl" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_ypprdwsl_4[] = {"standing_still", "moving_forward", "moving_backward"};
constexpr const char* kAutoOpts__TZE200_ypprdwsl_6[] = {"default", "area", "toilet", "bedroom", "parlour", "office", "hotel"};
constexpr const char* kAutoOpts__TZE200_ypprdwsl_7[] = {"ON", "OFF"};
constexpr const char* kAutoOpts__TZE200_ypprdwsl_10[] = {"none", "maybe_fall", "fall"};
constexpr Expose kAutoExposes__TZE200_ypprdwsl[] = {
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"presence", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"motion_speed", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"motion_direction", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_ypprdwsl_4, 3},
    {"radar_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 10, 1},
    {"radar_scene", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_ypprdwsl_6, 7},
    {"tumble_switch", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_ypprdwsl_7, 2},
    {"fall_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 10, 1},
    {"tumble_alarm_time", ExposeType::Numeric, Access::StateSet, "min", nullptr, nullptr, 0, ExposeCategory::State, 1, 5, 1},
    {"fall_down_status", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_ypprdwsl_10, 3},
    {"static_dwell_alarm", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_ypprdwsl{
    .zigbee_models=kM__TZE200_ypprdwsl,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_ypprdwsl,.manufacturer_names_count=1,
    .model="TS0601__TZE200_ypprdwsl",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_ypprdwsl,.exposes_count=sizeof(kAutoExposes__TZE200_ypprdwsl)/sizeof(kAutoExposes__TZE200_ypprdwsl[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_ypprdwsl,
    .from_zigbee_count=sizeof(kFz__TZE200_ypprdwsl)/sizeof(kFz__TZE200_ypprdwsl[0]),
    .to_zigbee=kTz__TZE200_ypprdwsl,
    .to_zigbee_count=sizeof(kTz__TZE200_ypprdwsl)/sizeof(kTz__TZE200_ypprdwsl[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
