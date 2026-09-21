// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_vizxbhco).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_vizxbhco_light_type_l1_dp4[] = {
    { 0, "led" },
    { 1, "incandescent" },
    { 2, "halogen" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_vizxbhco_light_type_l2_dp10[] = {
    { 0, "led" },
    { 1, "incandescent" },
    { 2, "halogen" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_vizxbhco_power_on_behavior_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_vizxbhco_light_type_l3_dp18[] = {
    { 0, "led" },
    { 1, "incandescent" },
    { 2, "halogen" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_vizxbhco_backlight_mode_dp21[] = {
    { 0, "off" },
    { 1, "normal" },
    { 2, "inverted" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_vizxbhco[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "brightness_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "min_brightness_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "light_type_l1", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_vizxbhco_light_type_l1_dp4, sizeof(kEnum__TZE200_vizxbhco_light_type_l1_dp4)/sizeof(kEnum__TZE200_vizxbhco_light_type_l1_dp4[0]) },
    { 5, "max_brightness_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 6, "countdown_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 8, "brightness_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "min_brightness_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 10, "light_type_l2", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_vizxbhco_light_type_l2_dp10, sizeof(kEnum__TZE200_vizxbhco_light_type_l2_dp10)/sizeof(kEnum__TZE200_vizxbhco_light_type_l2_dp10[0]) },
    { 11, "max_brightness_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 12, "countdown_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_vizxbhco_power_on_behavior_dp14, sizeof(kEnum__TZE200_vizxbhco_power_on_behavior_dp14)/sizeof(kEnum__TZE200_vizxbhco_power_on_behavior_dp14[0]) },
    { 15, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 16, "brightness_l3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "min_brightness_l3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "light_type_l3", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_vizxbhco_light_type_l3_dp18, sizeof(kEnum__TZE200_vizxbhco_light_type_l3_dp18)/sizeof(kEnum__TZE200_vizxbhco_light_type_l3_dp18[0]) },
    { 19, "max_brightness_l3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 20, "countdown_l3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 21, "backlight_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_vizxbhco_backlight_mode_dp21, sizeof(kEnum__TZE200_vizxbhco_backlight_mode_dp21)/sizeof(kEnum__TZE200_vizxbhco_backlight_mode_dp21[0]) },
    { 103, "backlight_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_vizxbhco{ kEntries__TZE200_vizxbhco, 21 };
constexpr FzConverter kFzDp__TZE200_vizxbhco{
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
    .user_config       = &kMap__TZE200_vizxbhco,
};
const FzConverter* const kFz__TZE200_vizxbhco[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_vizxbhco,
};
constexpr TzConverter kTzDp__TZE200_vizxbhco{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_vizxbhco,
};
const TzConverter* const kTz__TZE200_vizxbhco[] = { &kTzDp__TZE200_vizxbhco };
constexpr const char* kM__TZE200_vizxbhco[] = { "TS0601" };
constexpr const char* kN__TZE200_vizxbhco[] = { "_TZE200_vizxbhco" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_vizxbhco_15[] = {"led", "incandescent", "halogen"};
constexpr const char* kAutoOpts__TZE200_vizxbhco_16[] = {"led", "incandescent", "halogen"};
constexpr const char* kAutoOpts__TZE200_vizxbhco_17[] = {"led", "incandescent", "halogen"};
constexpr const char* kAutoOpts__TZE200_vizxbhco_18[] = {"off", "previous", "on"};
constexpr const char* kAutoOpts__TZE200_vizxbhco_19[] = {"off", "normal", "inverted"};
constexpr const char* kAutoOpts__TZE200_vizxbhco_20[] = {"red", "blue", "green", "white", "yellow", "magenta", "cyan", "warm_white"};
constexpr Expose kAutoExposes__TZE200_vizxbhco[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 254, 0},
    {"min_brightness_l1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 255, 0},
    {"max_brightness_l1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 255, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l2", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 254, 0},
    {"min_brightness_l2", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 255, 0},
    {"max_brightness_l2", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 255, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l3", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 254, 0},
    {"min_brightness_l3", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 255, 0},
    {"max_brightness_l3", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 255, 0},
    {"countdown_l1", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l2", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"countdown_l3", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"light_type_l1", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_vizxbhco_15, 3},
    {"light_type_l2", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_vizxbhco_16, 3},
    {"light_type_l3", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_vizxbhco_17, 3},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_vizxbhco_18, 3, ExposeCategory::Config},
    {"backlight_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_vizxbhco_19, 3, ExposeCategory::Config},
    {"backlight_color", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_vizxbhco_20, 8},
    {"backlight_brightness", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 1},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_vizxbhco[] = {
    {"Moes","ZS-SR-EUD-3"},
    {"Zemismart","ZN2S-RS3E-DH"},
    {"Moes","ZS-EUD_3gang"},
};
extern const PreparedDefinition kDefGen__TZE200_vizxbhco{
    .zigbee_models=kM__TZE200_vizxbhco,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_vizxbhco,.manufacturer_names_count=1,
    .model="TS0601__TZE200_vizxbhco",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_vizxbhco,.exposes_count=sizeof(kAutoExposes__TZE200_vizxbhco)/sizeof(kAutoExposes__TZE200_vizxbhco[0]),
    .white_labels=kWhiteLabels_Gen_TZE200_vizxbhco, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_vizxbhco)/sizeof(kWhiteLabels_Gen_TZE200_vizxbhco[0]),
    .from_zigbee=kFz__TZE200_vizxbhco,
    .from_zigbee_count=sizeof(kFz__TZE200_vizxbhco)/sizeof(kFz__TZE200_vizxbhco[0]),
    .to_zigbee=kTz__TZE200_vizxbhco,
    .to_zigbee_count=sizeof(kTz__TZE200_vizxbhco)/sizeof(kTz__TZE200_vizxbhco[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
