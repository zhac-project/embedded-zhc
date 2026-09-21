// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_7zazvlyn).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_7zazvlyn_cover_state_l1_dp133[] = {
    { 0, "open" },
    { 1, "stop" },
    { 2, "close" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_7zazvlyn_cover_state_l2_dp134[] = {
    { 0, "open" },
    { 1, "stop" },
    { 2, "close" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_7zazvlyn_cover_state_l3_dp135[] = {
    { 0, "open" },
    { 1, "stop" },
    { 2, "close" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_7zazvlyn_cover_state_l4_dp136[] = {
    { 0, "open" },
    { 1, "stop" },
    { 2, "close" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_7zazvlyn[] = {
    { 1, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 2, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 6, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "led_bright_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "led_bright_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "led_bright_l3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "led_bright_l4", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "led_warm_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 110, "led_warm_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "led_warm_l3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 112, "led_warm_l4", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 113, "cover_position_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 114, "cover_position_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 115, "cover_position_l3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 116, "cover_position_l4", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 117, "led_switch_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 118, "led_switch_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 119, "led_switch_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 120, "led_switch_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 121, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 122, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 123, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 124, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 125, "led_switch_name_l1", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 126, "led_switch_name_l2", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 127, "led_switch_name_l3", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 128, "led_switch_name_l4", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 129, "cover_name_l1", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 130, "cover_name_l2", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 131, "cover_name_l3", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 132, "cover_name_l4", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 133, "cover_state_l1", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_7zazvlyn_cover_state_l1_dp133, sizeof(kEnum__TZE284_7zazvlyn_cover_state_l1_dp133)/sizeof(kEnum__TZE284_7zazvlyn_cover_state_l1_dp133[0]) },
    { 134, "cover_state_l2", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_7zazvlyn_cover_state_l2_dp134, sizeof(kEnum__TZE284_7zazvlyn_cover_state_l2_dp134)/sizeof(kEnum__TZE284_7zazvlyn_cover_state_l2_dp134[0]) },
    { 135, "cover_state_l3", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_7zazvlyn_cover_state_l3_dp135, sizeof(kEnum__TZE284_7zazvlyn_cover_state_l3_dp135)/sizeof(kEnum__TZE284_7zazvlyn_cover_state_l3_dp135[0]) },
    { 136, "cover_state_l4", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_7zazvlyn_cover_state_l4_dp136, sizeof(kEnum__TZE284_7zazvlyn_cover_state_l4_dp136)/sizeof(kEnum__TZE284_7zazvlyn_cover_state_l4_dp136[0]) },
    { 137, "switch_name_l1", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 138, "switch_name_l2", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 139, "switch_name_l3", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 140, "switch_name_l4", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 141, "scene_name_l1", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 142, "scene_name_l2", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 143, "scene_name_l3", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 144, "scene_name_l4", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 145, "scene_name_l5", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 146, "scene_name_l6", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 147, "scene_name_l7", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 148, "scene_name_l8", ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0 },
    { 149, "backlight_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_7zazvlyn{ kEntries__TZE284_7zazvlyn, 53 };
constexpr FzConverter kFzDp__TZE284_7zazvlyn{
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
    .user_config       = &kMap__TZE284_7zazvlyn,
};
const FzConverter* const kFz__TZE284_7zazvlyn[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_7zazvlyn,
};
constexpr TzConverter kTzDp__TZE284_7zazvlyn{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_7zazvlyn,
};
const TzConverter* const kTz__TZE284_7zazvlyn[] = { &kTzDp__TZE284_7zazvlyn };
constexpr const char* kM__TZE284_7zazvlyn[] = { "TS0601" };
constexpr const char* kN__TZE284_7zazvlyn[] = { "_TZE284_7zazvlyn" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE284_7zazvlyn_37[] = {"open", "stop", "close"};
constexpr const char* kAutoOpts__TZE284_7zazvlyn_39[] = {"open", "stop", "close"};
constexpr const char* kAutoOpts__TZE284_7zazvlyn_41[] = {"open", "stop", "close"};
constexpr const char* kAutoOpts__TZE284_7zazvlyn_43[] = {"open", "stop", "close"};
constexpr const char* kAutoOpts__TZE284_7zazvlyn_46[] = {"heavy_rain", "thunderstorm", "dust_storm", "light_snow", "snow", "freezing_fog", "shower", "floating_dust", "thunder_and_lighting", "light_shower", "rain", "rain_and_snow", "dust_bowl", "ice_pellets", "strong_dust_storms", "sandy", "light_to_moderate_rain", "mostly_sunny", "sunny", "haze", "heavy_shower", "heavy_snow", "very_heavy_rain", "blizzard", "ice_pod", "light_to_moderate_snow", "few_clouds", "light_snow_showers", "moderate_snow", "cloudy", "icy_needles", "thunderstorm_with_ice_pods", "freezing_rain", "snow_shower", "light_rain", "thunder", "moderate_rain", "moderate_to_heavy_rain"};
constexpr const char* kAutoOpts__TZE284_7zazvlyn_47[] = {"scene_1", "scene_2", "scene_3", "scene_4", "scene_5", "scene_6", "scene_7", "scene_8"};
constexpr Expose kAutoExposes__TZE284_7zazvlyn[] = {
    {"backlight_switch", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch_name_l1", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch_name_l2", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch_name_l3", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch_name_l4", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scene_name_l1", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scene_name_l2", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scene_name_l3", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scene_name_l4", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scene_name_l5", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scene_name_l6", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scene_name_l7", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scene_name_l8", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"cover_name_l1", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"cover_name_l2", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"cover_name_l3", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"cover_name_l4", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_switch_name_l1", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_switch_name_l2", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_switch_name_l3", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_switch_name_l4", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_switch_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_warm_l1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 100, 0},
    {"led_bright_l1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 100, 0},
    {"led_switch_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_warm_l2", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 100, 0},
    {"led_bright_l2", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 100, 0},
    {"led_switch_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_warm_l3", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 100, 0},
    {"led_bright_l3", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 100, 0},
    {"led_switch_l4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_warm_l4", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 100, 0},
    {"led_bright_l4", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 100, 0},
    {"cover_state_l1", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_7zazvlyn_37, 3},
    {"cover_position_l1", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"cover_state_l2", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_7zazvlyn_39, 3},
    {"cover_position_l2", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"cover_state_l3", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_7zazvlyn_41, 3},
    {"cover_position_l3", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"cover_state_l4", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_7zazvlyn_43, 3},
    {"cover_position_l4", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"temperature_1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, -65, 99, 0},
    {"condition_1", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_7zazvlyn_46, 38},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE284_7zazvlyn_47, 8, ExposeCategory::Diagnostic},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_7zazvlyn{
    .zigbee_models=kM__TZE284_7zazvlyn,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_7zazvlyn,.manufacturer_names_count=1,
    .model="TS0601__TZE284_7zazvlyn",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_7zazvlyn,.exposes_count=sizeof(kAutoExposes__TZE284_7zazvlyn)/sizeof(kAutoExposes__TZE284_7zazvlyn[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_7zazvlyn,
    .from_zigbee_count=sizeof(kFz__TZE284_7zazvlyn)/sizeof(kFz__TZE284_7zazvlyn[0]),
    .to_zigbee=kTz__TZE284_7zazvlyn,
    .to_zigbee_count=sizeof(kTz__TZE284_7zazvlyn)/sizeof(kTz__TZE284_7zazvlyn[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
