// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_atuj3i0w).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_atuj3i0w_mode_l1_dp18[] = {
    { 0, "switch_1" },
    { 1, "scene_1" },
    { 2, "smart_light_1" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_atuj3i0w_mode_l2_dp19[] = {
    { 0, "switch_1" },
    { 1, "scene_1" },
    { 2, "smart_light_1" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_atuj3i0w_mode_l3_dp20[] = {
    { 0, "switch_1" },
    { 1, "scene_1" },
    { 2, "smart_light_1" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_atuj3i0w_mode_l4_dp21[] = {
    { 0, "switch_1" },
    { 1, "scene_1" },
    { 2, "smart_light_1" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_atuj3i0w_indicator_switch_dp37[] = {
    { 0, "status" },
    { 1, "switch_position" },
    { 2, "off" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_atuj3i0w[] = {
    { 24, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 25, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 26, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 27, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 18, "mode_l1", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_atuj3i0w_mode_l1_dp18, sizeof(kEnum__TZE284_atuj3i0w_mode_l1_dp18)/sizeof(kEnum__TZE284_atuj3i0w_mode_l1_dp18[0]) },
    { 19, "mode_l2", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_atuj3i0w_mode_l2_dp19, sizeof(kEnum__TZE284_atuj3i0w_mode_l2_dp19)/sizeof(kEnum__TZE284_atuj3i0w_mode_l2_dp19[0]) },
    { 20, "mode_l3", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_atuj3i0w_mode_l3_dp20, sizeof(kEnum__TZE284_atuj3i0w_mode_l3_dp20)/sizeof(kEnum__TZE284_atuj3i0w_mode_l3_dp20[0]) },
    { 21, "mode_l4", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_atuj3i0w_mode_l4_dp21, sizeof(kEnum__TZE284_atuj3i0w_mode_l4_dp21)/sizeof(kEnum__TZE284_atuj3i0w_mode_l4_dp21[0]) },
    { 1, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 2, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "backlight", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 36, "backlight_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 37, "indicator_switch", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_atuj3i0w_indicator_switch_dp37, sizeof(kEnum__TZE284_atuj3i0w_indicator_switch_dp37)/sizeof(kEnum__TZE284_atuj3i0w_indicator_switch_dp37[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_atuj3i0w{ kEntries__TZE284_atuj3i0w, 15 };
constexpr FzConverter kFzDp__TZE284_atuj3i0w{
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
    .user_config       = &kMap__TZE284_atuj3i0w,
};
const FzConverter* const kFz__TZE284_atuj3i0w[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_atuj3i0w,
};
constexpr TzConverter kTzDp__TZE284_atuj3i0w{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_atuj3i0w,
};
const TzConverter* const kTz__TZE284_atuj3i0w[] = { &kTzDp__TZE284_atuj3i0w };
constexpr const char* kM__TZE284_atuj3i0w[] = { "TS0601" };
constexpr const char* kN__TZE284_atuj3i0w[] = { "_TZE284_atuj3i0w" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE284_atuj3i0w_12[] = {"switch_1", "scene_1", "smart_light_1"};
constexpr const char* kAutoOpts__TZE284_atuj3i0w_13[] = {"switch_1", "scene_1", "smart_light_1"};
constexpr const char* kAutoOpts__TZE284_atuj3i0w_14[] = {"switch_1", "scene_1", "smart_light_1"};
constexpr const char* kAutoOpts__TZE284_atuj3i0w_15[] = {"switch_1", "scene_1", "smart_light_1"};
constexpr const char* kAutoOpts__TZE284_atuj3i0w_17[] = {"status", "switch_position", "off"};
constexpr const char* kAutoOpts__TZE284_atuj3i0w_20[] = {"sunny", "heavy_rain", "cloudy", "sandstorm", "light_snow", "snow", "freezing_fog", "rainstorm", "shower", "dust", "spit", "sleet", "yin", "freezing_rain", "rain", "fog", "heavy_shower", "heavy_snow", "heavy_downpour", "blizzard", "hailstone", "snow_shower", "haze", "thunder_shower"};
constexpr const char* kAutoOpts__TZE284_atuj3i0w_21[] = {"scene_1", "scene_2", "scene_3", "scene_4"};
constexpr Expose kAutoExposes__TZE284_atuj3i0w[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"name_l1", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"name_l2", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"name_l3", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"name_l4", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scene_name_l1", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scene_name_l2", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scene_name_l3", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scene_name_l4", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"mode_l1", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_atuj3i0w_12, 3},
    {"mode_l2", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_atuj3i0w_13, 3},
    {"mode_l3", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_atuj3i0w_14, 3},
    {"mode_l4", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_atuj3i0w_15, 3},
    {"backlight", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"indicator_switch", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_atuj3i0w_17, 3},
    {"backlight_switch", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"temperature_1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, -65, 99, 0},
    {"condition_1", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_atuj3i0w_20, 24},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE284_atuj3i0w_21, 4, ExposeCategory::Diagnostic},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_atuj3i0w{
    .zigbee_models=kM__TZE284_atuj3i0w,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_atuj3i0w,.manufacturer_names_count=1,
    .model="TS0601__TZE284_atuj3i0w",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_atuj3i0w,.exposes_count=sizeof(kAutoExposes__TZE284_atuj3i0w)/sizeof(kAutoExposes__TZE284_atuj3i0w[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_atuj3i0w,
    .from_zigbee_count=sizeof(kFz__TZE284_atuj3i0w)/sizeof(kFz__TZE284_atuj3i0w[0]),
    .to_zigbee=kTz__TZE284_atuj3i0w,
    .to_zigbee_count=sizeof(kTz__TZE284_atuj3i0w)/sizeof(kTz__TZE284_atuj3i0w[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
