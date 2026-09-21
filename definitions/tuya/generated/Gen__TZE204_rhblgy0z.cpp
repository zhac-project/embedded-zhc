// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_rhblgy0z).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_rhblgy0z[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 2, "produced_energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 16, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 18, "meter_id", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 20, "clear_fault", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_rhblgy0z{ kEntries__TZE204_rhblgy0z, 5 };
constexpr FzConverter kFzDp__TZE204_rhblgy0z{
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
    .user_config       = &kMap__TZE204_rhblgy0z,
};
const FzConverter* const kFz__TZE204_rhblgy0z[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_rhblgy0z,
};
constexpr TzConverter kTzDp__TZE204_rhblgy0z{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_rhblgy0z,
};
const TzConverter* const kTz__TZE204_rhblgy0z[] = { &kTzDp__TZE204_rhblgy0z };
constexpr const char* kM__TZE204_rhblgy0z[] = { "TS0601" };
constexpr const char* kN__TZE204_rhblgy0z[] = { "_TZE204_rhblgy0z" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE204_rhblgy0z_6[] = {"clear", "over_current_threshold", "over_power_threshold", "over_voltage threshold", "wrong_frequency_threshold"};
constexpr const char* kAutoOpts__TZE204_rhblgy0z_7[] = {"not_set", "over_current_threshold", "over_voltage_threshold"};
constexpr const char* kAutoOpts__TZE204_rhblgy0z_10[] = {"not_set", "over_current_threshold", "over_voltage_threshold"};
constexpr Expose kAutoExposes__TZE204_rhblgy0z[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"produced_energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"fault", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_rhblgy0z_6, 5},
    {"threshold_1", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_rhblgy0z_7, 3},
    {"threshold_1_protection", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"threshold_1_value", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"threshold_2", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_rhblgy0z_10, 3},
    {"threshold_2_protection", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"threshold_2_value", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"clear_fault", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"meter_id", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_rhblgy0z[] = {
    {"XOCA","DAC2161C"},
};
extern const PreparedDefinition kDefGen__TZE204_rhblgy0z{
    .zigbee_models=kM__TZE204_rhblgy0z,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_rhblgy0z,.manufacturer_names_count=1,
    .model="TS0601__TZE204_rhblgy0z",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_rhblgy0z,.exposes_count=sizeof(kAutoExposes__TZE204_rhblgy0z)/sizeof(kAutoExposes__TZE204_rhblgy0z[0]),
    .white_labels=kWhiteLabels_Gen_TZE204_rhblgy0z, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_rhblgy0z)/sizeof(kWhiteLabels_Gen_TZE204_rhblgy0z[0]),
    .from_zigbee=kFz__TZE204_rhblgy0z,
    .from_zigbee_count=sizeof(kFz__TZE204_rhblgy0z)/sizeof(kFz__TZE204_rhblgy0z[0]),
    .to_zigbee=kTz__TZE204_rhblgy0z,
    .to_zigbee_count=sizeof(kTz__TZE204_rhblgy0z)/sizeof(kTz__TZE204_rhblgy0z[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
