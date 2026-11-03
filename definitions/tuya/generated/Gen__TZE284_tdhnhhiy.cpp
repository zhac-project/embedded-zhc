// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Nova Digital SYZB-6W 6-gang 4x4 switch with backlight (TS0601 /
// _TZE284_tdhnhhiy). Generated from TS0601_switch_6_gang; z2m v26.125.0 moves
// this fingerprint to its own nova_digital.ts definition: DP14
// power_on_behavior {off, on, previous} (was labelled "memory"), new DP16
// backlight_switch, configure binds genOnOff and forces mains power. The
// switch_6_gang white labels stay with that def upstream and are dropped here.
// Upstream dropped DP15 indicator_mode; kept, it only decodes what the device
// sends.
// z2m-source: nova_digital.ts #SYZB-6W.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_tdhnhhiy_power_on_behavior_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_tdhnhhiy_indicator_mode_dp15[] = {
    { 0, "none" },
    { 1, "relay" },
    { 2, "pos" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_tdhnhhiy[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 5, "state_l5", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 6, "state_l6", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 14, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_tdhnhhiy_power_on_behavior_dp14, sizeof(kEnum__TZE284_tdhnhhiy_power_on_behavior_dp14)/sizeof(kEnum__TZE284_tdhnhhiy_power_on_behavior_dp14[0]) },
    { 16, "backlight_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 15, "indicator_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_tdhnhhiy_indicator_mode_dp15, sizeof(kEnum__TZE284_tdhnhhiy_indicator_mode_dp15)/sizeof(kEnum__TZE284_tdhnhhiy_indicator_mode_dp15[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_tdhnhhiy{ kEntries__TZE284_tdhnhhiy, sizeof(kEntries__TZE284_tdhnhhiy)/sizeof(kEntries__TZE284_tdhnhhiy[0]) };
constexpr FzConverter kFzDp__TZE284_tdhnhhiy{
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
    .user_config       = &kMap__TZE284_tdhnhhiy,
};
const FzConverter* const kFz__TZE284_tdhnhhiy[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_tdhnhhiy,
};
constexpr TzConverter kTzDp__TZE284_tdhnhhiy{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_tdhnhhiy,
};
const TzConverter* const kTz__TZE284_tdhnhhiy[] = { &kTzDp__TZE284_tdhnhhiy };
constexpr const char* kM__TZE284_tdhnhhiy[] = { "TS0601" };
constexpr const char* kN__TZE284_tdhnhhiy[] = { "_TZE284_tdhnhhiy" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE284_tdhnhhiy_6[] = {"off", "on", "previous"};
constexpr const char* kAutoOpts__TZE284_tdhnhhiy_7[] = {"none", "relay", "pos"};
constexpr Expose kAutoExposes__TZE284_tdhnhhiy[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l5", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l6", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_tdhnhhiy_6, 3, ExposeCategory::Config},
    {"indicator_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_tdhnhhiy_7, 3, ExposeCategory::Config},
    {"backlight_switch", ExposeType::Binary, Access::StateSet, nullptr, "Backlight switch", nullptr, 0},
};
constexpr BindingSpec kBindings__TZE284_tdhnhhiy[] = { {1, 0x0006} };
// --- end auto-generated exposes ---
}  // namespace

extern const PreparedDefinition kDefGen__TZE284_tdhnhhiy{
    .zigbee_models=kM__TZE284_tdhnhhiy,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_tdhnhhiy,.manufacturer_names_count=1,
    .model="SYZB-6W",.vendor="Nova Digital",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_tdhnhhiy,.exposes_count=sizeof(kAutoExposes__TZE284_tdhnhhiy)/sizeof(kAutoExposes__TZE284_tdhnhhiy[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_tdhnhhiy,
    .from_zigbee_count=sizeof(kFz__TZE284_tdhnhhiy)/sizeof(kFz__TZE284_tdhnhhiy[0]),
    .to_zigbee=kTz__TZE284_tdhnhhiy,
    .to_zigbee_count=sizeof(kTz__TZE284_tdhnhhiy)/sizeof(kTz__TZE284_tdhnhhiy[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
    .bindings=kBindings__TZE284_tdhnhhiy,.bindings_count=sizeof(kBindings__TZE284_tdhnhhiy)/sizeof(kBindings__TZE284_tdhnhhiy[0]),
    .power_source_override=0x01 };
}  // namespace zhc::devices::tuya
