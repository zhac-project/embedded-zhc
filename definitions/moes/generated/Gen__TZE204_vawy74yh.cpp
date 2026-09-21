// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_vawy74yh).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_vawy74yh_battery_state_dp14[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_vawy74yh[] = {
    { 1, "smoke", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 14, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_vawy74yh_battery_state_dp14, sizeof(kEnum__TZE204_vawy74yh_battery_state_dp14)/sizeof(kEnum__TZE204_vawy74yh_battery_state_dp14[0]) },
    { 15, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "silence", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_vawy74yh{ kEntries__TZE204_vawy74yh, 4 };
constexpr FzConverter kFzDp__TZE204_vawy74yh{
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
    .user_config       = &kMap__TZE204_vawy74yh,
};
const FzConverter* const kFz__TZE204_vawy74yh[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_vawy74yh,
};
constexpr TzConverter kTzDp__TZE204_vawy74yh{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_vawy74yh,
};
const TzConverter* const kTz__TZE204_vawy74yh[] = { &kTzDp__TZE204_vawy74yh };
constexpr const char* kM__TZE204_vawy74yh[] = { "TS0601" };
constexpr const char* kN__TZE204_vawy74yh[] = { "_TZE204_vawy74yh" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE204_vawy74yh_2[] = {"low", "medium", "high"};
constexpr const char* kAutoOpts__TZE204_vawy74yh_4[] = {"checking", "check_success", "check_failure"};
constexpr Expose kAutoExposes__TZE204_vawy74yh[] = {
    {"smoke", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
    {"battery_state", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_vawy74yh_2, 3, ExposeCategory::Diagnostic},
    {"silence", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"self_test", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_vawy74yh_4, 3},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_vawy74yh{
    .zigbee_models=kM__TZE204_vawy74yh,.zigbee_models_count=sizeof(kM__TZE204_vawy74yh)/sizeof(kM__TZE204_vawy74yh[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_vawy74yh,.manufacturer_names_count=1,
    .model="TS0601__TZE204_vawy74yh",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_vawy74yh,.exposes_count=sizeof(kAutoExposes__TZE204_vawy74yh)/sizeof(kAutoExposes__TZE204_vawy74yh[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_vawy74yh,
    .from_zigbee_count=sizeof(kFz__TZE204_vawy74yh)/sizeof(kFz__TZE204_vawy74yh[0]),
    .to_zigbee=kTz__TZE204_vawy74yh,
    .to_zigbee_count=sizeof(kTz__TZE204_vawy74yh)/sizeof(kTz__TZE204_vawy74yh[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
