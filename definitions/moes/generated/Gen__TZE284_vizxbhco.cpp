// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_vizxbhco).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_vizxbhco_relay_status_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "memory" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_vizxbhco_light_mode_dp21[] = {
    { 0, "none" },
    { 1, "relay" },
    { 2, "pos" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_vizxbhco[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "brightness_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "brightness_min_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "brightness_max_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 6, "countdown_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 7, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 8, "brightness_2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "brightness_min_2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 11, "brightness_max_2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 12, "countdown_2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "relay_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_vizxbhco_relay_status_dp14, sizeof(kEnum__TZE284_vizxbhco_relay_status_dp14)/sizeof(kEnum__TZE284_vizxbhco_relay_status_dp14[0]) },
    { 15, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 16, "brightness_3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "brightness_min_3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 19, "brightness_max_3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 20, "countdown_3", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 21, "light_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_vizxbhco_light_mode_dp21, sizeof(kEnum__TZE284_vizxbhco_light_mode_dp21)/sizeof(kEnum__TZE284_vizxbhco_light_mode_dp21[0]) },
    { 26, "switch_backlight", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_vizxbhco{ kEntries__TZE284_vizxbhco, 18 };
constexpr FzConverter kFzDp__TZE284_vizxbhco{
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
    .user_config       = &kMap__TZE284_vizxbhco,
};
const FzConverter* const kFz__TZE284_vizxbhco[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_vizxbhco,
};
constexpr TzConverter kTzDp__TZE284_vizxbhco{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_vizxbhco,
};
const TzConverter* const kTz__TZE284_vizxbhco[] = { &kTzDp__TZE284_vizxbhco };
constexpr const char* kM__TZE284_vizxbhco[] = { "TS0601" };
constexpr const char* kN__TZE284_vizxbhco[] = { "_TZE284_vizxbhco" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE284_vizxbhco_11[] = {"off", "on", "memory"};
constexpr const char* kAutoOpts__TZE284_vizxbhco_16[] = {"none", "relay", "pos"};
constexpr Expose kAutoExposes__TZE284_vizxbhco[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 10, 1000, 1},
    {"brightness_min_1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 10, 1000, 1},
    {"brightness_max_1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 10, 1000, 1},
    {"countdown_1", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 86400, 1},
    {"brightness_2", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 10, 1000, 1},
    {"brightness_min_2", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 10, 1000, 1},
    {"brightness_max_2", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 10, 1000, 1},
    {"countdown_2", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 86400, 1},
    {"relay_status", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_vizxbhco_11, 3},
    {"brightness_3", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 10, 1000, 1},
    {"brightness_min_3", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 10, 1000, 1},
    {"brightness_max_3", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 10, 1000, 1},
    {"countdown_3", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 86400, 1},
    {"light_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_vizxbhco_16, 3},
    {"switch_backlight", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_vizxbhco{
    .zigbee_models=kM__TZE284_vizxbhco,.zigbee_models_count=sizeof(kM__TZE284_vizxbhco)/sizeof(kM__TZE284_vizxbhco[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_vizxbhco,.manufacturer_names_count=1,
    .model="TS0601__TZE284_vizxbhco",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_vizxbhco,.exposes_count=sizeof(kAutoExposes__TZE284_vizxbhco)/sizeof(kAutoExposes__TZE284_vizxbhco[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_vizxbhco,
    .from_zigbee_count=sizeof(kFz__TZE284_vizxbhco)/sizeof(kFz__TZE284_vizxbhco[0]),
    .to_zigbee=kTz__TZE284_vizxbhco,
    .to_zigbee_count=sizeof(kTz__TZE284_vizxbhco)/sizeof(kTz__TZE284_vizxbhco[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
