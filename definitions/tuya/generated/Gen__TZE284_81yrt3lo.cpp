// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_81yrt3lo).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_81yrt3lo[] = {
    { 111, "ac_frequency", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 112, "voltage", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 129, "update_frequency", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_81yrt3lo{ kEntries__TZE284_81yrt3lo, 3 };
constexpr FzConverter kFzDp__TZE284_81yrt3lo{
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
    .user_config       = &kMap__TZE284_81yrt3lo,
};
const FzConverter* const kFz__TZE284_81yrt3lo[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_81yrt3lo,
};
constexpr TzConverter kTzDp__TZE284_81yrt3lo{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_81yrt3lo,
};
const TzConverter* const kTz__TZE284_81yrt3lo[] = { &kTzDp__TZE284_81yrt3lo };
constexpr const char* kM__TZE284_81yrt3lo[] = { "TS0601" };
constexpr const char* kN__TZE284_81yrt3lo[] = { "_TZE284_81yrt3lo" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE284_81yrt3lo_9[] = {"consuming", "producing", "sign"};
constexpr const char* kAutoOpts__TZE284_81yrt3lo_10[] = {"consuming", "producing", "sign"};
constexpr Expose kAutoExposes__TZE284_81yrt3lo[] = {
    {"ac_frequency", ExposeType::Numeric, Access::State, "Hz", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"power_a", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_b", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_ab", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"current_a", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_b", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"power_factor_a", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"power_factor_b", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"energy_flow_a", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE284_81yrt3lo_9, 3},
    {"energy_flow_b", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE284_81yrt3lo_10, 3},
    {"energy_a", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"energy_b", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"energy_produced_a", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"energy_produced_b", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"update_frequency", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 3, 60, 0},
    {"timestamp_a", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
    {"timestamp_b", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_81yrt3lo{
    .zigbee_models=kM__TZE284_81yrt3lo,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_81yrt3lo,.manufacturer_names_count=1,
    .model="TS0601__TZE284_81yrt3lo",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_81yrt3lo,.exposes_count=sizeof(kAutoExposes__TZE284_81yrt3lo)/sizeof(kAutoExposes__TZE284_81yrt3lo[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_81yrt3lo,
    .from_zigbee_count=sizeof(kFz__TZE284_81yrt3lo)/sizeof(kFz__TZE284_81yrt3lo[0]),
    .to_zigbee=kTz__TZE284_81yrt3lo,
    .to_zigbee_count=sizeof(kTz__TZE284_81yrt3lo)/sizeof(kTz__TZE284_81yrt3lo[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
