// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_mvtclclq).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_mvtclclq_relay_status_dp14[] = {
    { 0, "memory" },
    { 1, "on" },
    { 2, "off" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_mvtclclq[] = {
    { 1, "state_usb_a", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_usb_c", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_plug_1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "state_plug_2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 7, "countdown_usb_a", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "countdown_usb_c", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "countdown_plug_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 10, "countdown_plug_2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "relay_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_mvtclclq_relay_status_dp14, sizeof(kEnum__TZE284_mvtclclq_relay_status_dp14)/sizeof(kEnum__TZE284_mvtclclq_relay_status_dp14[0]) },
    { 16, "switch_backlight", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 21, "current", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 22, "power", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 23, "voltage", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 105, "produced_energy", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 106, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_mvtclclq{ kEntries__TZE284_mvtclclq, 15 };
constexpr FzConverter kFzDp__TZE284_mvtclclq{
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
    .user_config       = &kMap__TZE284_mvtclclq,
};
const FzConverter* const kFz__TZE284_mvtclclq[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_mvtclclq,
};
constexpr TzConverter kTzDp__TZE284_mvtclclq{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_mvtclclq,
};
const TzConverter* const kTz__TZE284_mvtclclq[] = { &kTzDp__TZE284_mvtclclq };
constexpr const char* kM__TZE284_mvtclclq[] = { "TS0601" };
constexpr const char* kN__TZE284_mvtclclq[] = { "_TZE284_mvtclclq" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE284_mvtclclq_8[] = {"memory", "on", "off"};
constexpr Expose kAutoExposes__TZE284_mvtclclq[] = {
    {"state_usb_a", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_usb_c", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_plug_1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_plug_2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"countdown_usb_a", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 86400, 1},
    {"countdown_usb_c", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 86400, 1},
    {"countdown_plug_1", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 86400, 1},
    {"countdown_plug_2", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 86400, 1},
    {"relay_status", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_mvtclclq_8, 3},
    {"switch_backlight", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"produced_energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_mvtclclq{
    .zigbee_models=kM__TZE284_mvtclclq,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_mvtclclq,.manufacturer_names_count=1,
    .model="TS0601__TZE284_mvtclclq",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_mvtclclq,.exposes_count=sizeof(kAutoExposes__TZE284_mvtclclq)/sizeof(kAutoExposes__TZE284_mvtclclq[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_mvtclclq,
    .from_zigbee_count=sizeof(kFz__TZE284_mvtclclq)/sizeof(kFz__TZE284_mvtclclq[0]),
    .to_zigbee=kTz__TZE284_mvtclclq,
    .to_zigbee_count=sizeof(kTz__TZE284_mvtclclq)/sizeof(kTz__TZE284_mvtclclq[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
