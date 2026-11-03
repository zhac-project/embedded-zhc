// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Nova Digital ZTS-8W-B 8-gang switch (TS0601 / _TZE204_nvxorhcj).
// Generated from TS0601_switch_8 (relays only, DPs 1-6, 101, 102); z2m
// v26.125.0 moves the fingerprint to its own nova_digital.ts definition and
// adds DP14 power_on_behavior {off, on, previous} and DP15 backlight_mode
// {off, relay, position}.
// z2m-source: nova_digital.ts #ZTS-8W-B.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kPowerOnBehavior[] = { {0, "off"}, {1, "on"}, {2, "previous"} };
constexpr ::zhc::tuya::TuyaEnumEntry kBacklightMode[] = { {0, "off"}, {1, "relay"}, {2, "position"} };

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_nvxorhcj[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 5, "state_l5", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 6, "state_l6", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 101, "state_l7", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "state_l8", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    ::zhc::tuya::dp::enum_lookup(14, "power_on_behavior", kPowerOnBehavior, 3),
    ::zhc::tuya::dp::enum_lookup(15, "backlight_mode", kBacklightMode, 3),
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_nvxorhcj{ kEntries__TZE204_nvxorhcj, sizeof(kEntries__TZE204_nvxorhcj)/sizeof(kEntries__TZE204_nvxorhcj[0]) };
constexpr FzConverter kFzDp__TZE204_nvxorhcj{
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
    .user_config       = &kMap__TZE204_nvxorhcj,
};
const FzConverter* const kFz__TZE204_nvxorhcj[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_nvxorhcj,
};
constexpr TzConverter kTzDp__TZE204_nvxorhcj{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_nvxorhcj,
};
const TzConverter* const kTz__TZE204_nvxorhcj[] = { &kTzDp__TZE204_nvxorhcj };
constexpr const char* kM__TZE204_nvxorhcj[] = { "TS0601" };
constexpr const char* kN__TZE204_nvxorhcj[] = { "_TZE204_nvxorhcj" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kPowerOnOpts[] = { "off", "on", "previous" };
constexpr const char* kBacklightOpts[] = { "off", "relay", "position" };
constexpr Expose kAutoExposes__TZE204_nvxorhcj[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l5", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l6", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l7", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l8", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, "Power on behavior status",
     kPowerOnOpts, 3, ExposeCategory::Config},
    {"backlight_mode", ExposeType::Enum, Access::StateSet, nullptr, "Status indicator",
     kBacklightOpts, 3, ExposeCategory::Config},
};
// --- end auto-generated exposes ---
}  // namespace

extern const PreparedDefinition kDefGen__TZE204_nvxorhcj{
    .zigbee_models=kM__TZE204_nvxorhcj,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_nvxorhcj,.manufacturer_names_count=1,
    .model="ZTS-8W-B",.vendor="Nova Digital",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_nvxorhcj,.exposes_count=sizeof(kAutoExposes__TZE204_nvxorhcj)/sizeof(kAutoExposes__TZE204_nvxorhcj[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_nvxorhcj,
    .from_zigbee_count=sizeof(kFz__TZE204_nvxorhcj)/sizeof(kFz__TZE204_nvxorhcj[0]),
    .to_zigbee=kTz__TZE204_nvxorhcj,
    .to_zigbee_count=sizeof(kTz__TZE204_nvxorhcj)/sizeof(kTz__TZE204_nvxorhcj[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
