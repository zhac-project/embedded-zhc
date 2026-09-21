// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_fjjbhx9d).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_fjjbhx9d[] = {
    { 6, "countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 12, "countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_fjjbhx9d{ kEntries__TZE200_fjjbhx9d, 2 };
constexpr FzConverter kFzDp__TZE200_fjjbhx9d{
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
    .user_config       = &kMap__TZE200_fjjbhx9d,
};
const FzConverter* const kFz__TZE200_fjjbhx9d[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_fjjbhx9d,
};
constexpr TzConverter kTzDp__TZE200_fjjbhx9d{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_fjjbhx9d,
};
const TzConverter* const kTz__TZE200_fjjbhx9d[] = { &kTzDp__TZE200_fjjbhx9d };
constexpr const char* kM__TZE200_fjjbhx9d[] = { "TS0601" };
constexpr const char* kN__TZE200_fjjbhx9d[] = { "_TZE200_fjjbhx9d" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_fjjbhx9d_10[] = {"off", "on", "previous"};
constexpr const char* kAutoOpts__TZE200_fjjbhx9d_11[] = {"off", "normal", "inverted"};
constexpr Expose kAutoExposes__TZE200_fjjbhx9d[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 254, 0},
    {"min_brightness_l1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 255, 0},
    {"max_brightness_l1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 255, 0},
    {"countdown_l1", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l2", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 254, 0},
    {"min_brightness_l2", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 255, 0},
    {"max_brightness_l2", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 255, 0},
    {"countdown_l2", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_fjjbhx9d_10, 3, ExposeCategory::Config},
    {"backlight_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_fjjbhx9d_11, 3, ExposeCategory::Config},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_fjjbhx9d[] = {
    {"Moes","ZS-SR-EUD-2"},
    {"Moes","ZS-EUD_2gang"},
    {"Moes","MS-105B"},
};
extern const PreparedDefinition kDefGen__TZE200_fjjbhx9d{
    .zigbee_models=kM__TZE200_fjjbhx9d,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_fjjbhx9d,.manufacturer_names_count=1,
    .model="TS0601__TZE200_fjjbhx9d",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_fjjbhx9d,.exposes_count=sizeof(kAutoExposes__TZE200_fjjbhx9d)/sizeof(kAutoExposes__TZE200_fjjbhx9d[0]),
    .white_labels=kWhiteLabels_Gen_TZE200_fjjbhx9d, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_fjjbhx9d)/sizeof(kWhiteLabels_Gen_TZE200_fjjbhx9d[0]),
    .from_zigbee=kFz__TZE200_fjjbhx9d,
    .from_zigbee_count=sizeof(kFz__TZE200_fjjbhx9d)/sizeof(kFz__TZE200_fjjbhx9d[0]),
    .to_zigbee=kTz__TZE200_fjjbhx9d,
    .to_zigbee_count=sizeof(kTz__TZE200_fjjbhx9d)/sizeof(kTz__TZE200_fjjbhx9d[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
