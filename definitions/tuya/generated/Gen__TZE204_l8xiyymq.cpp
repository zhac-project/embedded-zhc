// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_l8xiyymq).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_l8xiyymq_power_on_behavior_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_l8xiyymq_switch_type_dp107[] = {
    { 0, "momentary" },
    { 1, "toggle" },
    { 2, "state" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_l8xiyymq_switch_mode_l1_l2_dp113[] = {
    { 0, "switch" },
    { 1, "scene" },
    { 2, "light" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_l8xiyymq_switch_mode_l3_l4_dp114[] = {
    { 0, "switch" },
    { 1, "scene" },
    { 2, "light" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_l8xiyymq_switch_mode_l5_l6_dp115[] = {
    { 0, "switch" },
    { 1, "scene" },
    { 2, "light" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_l8xiyymq[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 5, "state_l5", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 6, "state_l6", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 13, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 14, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_l8xiyymq_power_on_behavior_dp14, sizeof(kEnum__TZE204_l8xiyymq_power_on_behavior_dp14)/sizeof(kEnum__TZE204_l8xiyymq_power_on_behavior_dp14[0]) },
    { 107, "switch_type", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_l8xiyymq_switch_type_dp107, sizeof(kEnum__TZE204_l8xiyymq_switch_type_dp107)/sizeof(kEnum__TZE204_l8xiyymq_switch_type_dp107[0]) },
    { 113, "switch_mode_l1_l2", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_l8xiyymq_switch_mode_l1_l2_dp113, sizeof(kEnum__TZE204_l8xiyymq_switch_mode_l1_l2_dp113)/sizeof(kEnum__TZE204_l8xiyymq_switch_mode_l1_l2_dp113[0]) },
    { 114, "switch_mode_l3_l4", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_l8xiyymq_switch_mode_l3_l4_dp114, sizeof(kEnum__TZE204_l8xiyymq_switch_mode_l3_l4_dp114)/sizeof(kEnum__TZE204_l8xiyymq_switch_mode_l3_l4_dp114[0]) },
    { 115, "switch_mode_l5_l6", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_l8xiyymq_switch_mode_l5_l6_dp115, sizeof(kEnum__TZE204_l8xiyymq_switch_mode_l5_l6_dp115)/sizeof(kEnum__TZE204_l8xiyymq_switch_mode_l5_l6_dp115[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_l8xiyymq{ kEntries__TZE204_l8xiyymq, 12 };
constexpr FzConverter kFzDp__TZE204_l8xiyymq{
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
    .user_config       = &kMap__TZE204_l8xiyymq,
};
const FzConverter* const kFz__TZE204_l8xiyymq[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_l8xiyymq,
};
constexpr TzConverter kTzDp__TZE204_l8xiyymq{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_l8xiyymq,
};
const TzConverter* const kTz__TZE204_l8xiyymq[] = { &kTzDp__TZE204_l8xiyymq };
constexpr const char* kM__TZE204_l8xiyymq[] = { "TS0601" };
constexpr const char* kN__TZE204_l8xiyymq[] = { "_TZE204_l8xiyymq" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE204_l8xiyymq_1[] = {"switch", "curtain"};
constexpr const char* kAutoOpts__TZE204_l8xiyymq_4[] = {"switch", "curtain"};
constexpr const char* kAutoOpts__TZE204_l8xiyymq_7[] = {"switch", "curtain"};
constexpr const char* kAutoOpts__TZE204_l8xiyymq_10[] = {"toggle", "state", "momentary"};
constexpr const char* kAutoOpts__TZE204_l8xiyymq_11[] = {"off", "on", "previous"};
constexpr Expose kAutoExposes__TZE204_l8xiyymq[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch_mode_l1_l2", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_l8xiyymq_1, 2, ExposeCategory::Config},
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch_mode_l3_l4", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_l8xiyymq_4, 2, ExposeCategory::Config},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch_mode_l5_l6", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_l8xiyymq_7, 2, ExposeCategory::Config},
    {"state_l5", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l6", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch_type", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_l8xiyymq_10, 3, ExposeCategory::Config},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE204_l8xiyymq_11, 3, ExposeCategory::Config},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_l8xiyymq{
    .zigbee_models=kM__TZE204_l8xiyymq,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_l8xiyymq,.manufacturer_names_count=1,
    .model="TS0601__TZE204_l8xiyymq",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_l8xiyymq,.exposes_count=sizeof(kAutoExposes__TZE204_l8xiyymq)/sizeof(kAutoExposes__TZE204_l8xiyymq[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_l8xiyymq,
    .from_zigbee_count=sizeof(kFz__TZE204_l8xiyymq)/sizeof(kFz__TZE204_l8xiyymq[0]),
    .to_zigbee=kTz__TZE204_l8xiyymq,
    .to_zigbee_count=sizeof(kTz__TZE204_l8xiyymq)/sizeof(kTz__TZE204_l8xiyymq[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
