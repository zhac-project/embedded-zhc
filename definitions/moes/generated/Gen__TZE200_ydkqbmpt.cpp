// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_ydkqbmpt).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_ydkqbmpt_mode_l1_dp18[] = {
    { 0, "switch_1" },
    { 1, "scene_1" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_ydkqbmpt_indicator_status_dp37[] = {
    { 0, "off" },
    { 1, "relay" },
    { 2, "invert" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_ydkqbmpt_power_on_behavior_dp38[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_ydkqbmpt[] = {
    { 1, "action", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "mode_l1", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_ydkqbmpt_mode_l1_dp18, sizeof(kEnum__TZE200_ydkqbmpt_mode_l1_dp18)/sizeof(kEnum__TZE200_ydkqbmpt_mode_l1_dp18[0]) },
    { 24, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 30, "countdown_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 36, "backlight_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 37, "indicator_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_ydkqbmpt_indicator_status_dp37, sizeof(kEnum__TZE200_ydkqbmpt_indicator_status_dp37)/sizeof(kEnum__TZE200_ydkqbmpt_indicator_status_dp37[0]) },
    { 38, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_ydkqbmpt_power_on_behavior_dp38, sizeof(kEnum__TZE200_ydkqbmpt_power_on_behavior_dp38)/sizeof(kEnum__TZE200_ydkqbmpt_power_on_behavior_dp38[0]) },
    { 103, "induction_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 105, "momentary_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_ydkqbmpt{ kEntries__TZE200_ydkqbmpt, 9 };
constexpr FzConverter kFzDp__TZE200_ydkqbmpt{
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
    .user_config       = &kMap__TZE200_ydkqbmpt,
};
const FzConverter* const kFz__TZE200_ydkqbmpt[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_ydkqbmpt,
};
constexpr TzConverter kTzDp__TZE200_ydkqbmpt{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_ydkqbmpt,
};
const TzConverter* const kTz__TZE200_ydkqbmpt[] = { &kTzDp__TZE200_ydkqbmpt };
constexpr const char* kM__TZE200_ydkqbmpt[] = { "TS0601" };
constexpr const char* kN__TZE200_ydkqbmpt[] = { "_TZE200_ydkqbmpt" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_ydkqbmpt_4[] = {"off", "previous", "on"};
constexpr const char* kAutoOpts__TZE200_ydkqbmpt_5[] = {"switch_1", "scene_1"};
constexpr const char* kAutoOpts__TZE200_ydkqbmpt_6[] = {"off", "relay", "invert"};
constexpr const char* kAutoOpts__TZE200_ydkqbmpt_7[] = {"ON", "OFF"};
constexpr const char* kAutoOpts__TZE200_ydkqbmpt_8[] = {"Gear 0", "Gear 1", "Gear 2", "Gear 3"};
constexpr const char* kAutoOpts__TZE200_ydkqbmpt_9[] = {"scene_1"};
constexpr Expose kAutoExposes__TZE200_ydkqbmpt[] = {
    {"backlight_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"countdown_l1", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"momentary_1", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 3600, 1},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_ydkqbmpt_4, 3, ExposeCategory::Config},
    {"mode_l1", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_ydkqbmpt_5, 2},
    {"indicator_status", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_ydkqbmpt_6, 3},
    {"induction_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_ydkqbmpt_7, 2},
    {"vibration_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_ydkqbmpt_8, 4},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_ydkqbmpt_9, 1, ExposeCategory::Diagnostic},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_ydkqbmpt{
    .zigbee_models=kM__TZE200_ydkqbmpt,.zigbee_models_count=sizeof(kM__TZE200_ydkqbmpt)/sizeof(kM__TZE200_ydkqbmpt[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_ydkqbmpt,.manufacturer_names_count=1,
    .model="TS0601__TZE200_ydkqbmpt",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_ydkqbmpt,.exposes_count=sizeof(kAutoExposes__TZE200_ydkqbmpt)/sizeof(kAutoExposes__TZE200_ydkqbmpt[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_ydkqbmpt,
    .from_zigbee_count=sizeof(kFz__TZE200_ydkqbmpt)/sizeof(kFz__TZE200_ydkqbmpt[0]),
    .to_zigbee=kTz__TZE200_ydkqbmpt,
    .to_zigbee_count=sizeof(kTz__TZE200_ydkqbmpt)/sizeof(kTz__TZE200_ydkqbmpt[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
