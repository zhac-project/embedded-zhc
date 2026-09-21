// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_qaxkdgyt).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_qaxkdgyt_self_test_dp9[] = {
    { 0, "checking" },
    { 1, "check_success" },
    { 2, "check_failure" },
    { 3, "others" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_qaxkdgyt[] = {
    { 1, "gas", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 2, "gas_value", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "carbon_monoxide", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 19, "co", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "silence", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "self_test", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_qaxkdgyt_self_test_dp9, sizeof(kEnum__TZE204_qaxkdgyt_self_test_dp9)/sizeof(kEnum__TZE204_qaxkdgyt_self_test_dp9[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_qaxkdgyt{ kEntries__TZE204_qaxkdgyt, 6 };
constexpr FzConverter kFzDp__TZE204_qaxkdgyt{
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
    .user_config       = &kMap__TZE204_qaxkdgyt,
};
const FzConverter* const kFz__TZE204_qaxkdgyt[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_qaxkdgyt,
};
constexpr TzConverter kTzDp__TZE204_qaxkdgyt{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_qaxkdgyt,
};
const TzConverter* const kTz__TZE204_qaxkdgyt[] = { &kTzDp__TZE204_qaxkdgyt };
constexpr const char* kM__TZE204_qaxkdgyt[] = { "TS0601" };
constexpr const char* kN__TZE204_qaxkdgyt[] = { "_TZE204_qaxkdgyt" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE204_qaxkdgyt_5[] = {"checking", "check_success", "check_failure", "others"};
constexpr const char* kAutoOpts__TZE204_qaxkdgyt_6[] = {"none", "fault", "serious_fault", "sensor_fault", "probe_fault", "power_fault"};
constexpr Expose kAutoExposes__TZE204_qaxkdgyt[] = {
    {"carbon_monoxide", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"co", ExposeType::Numeric, Access::State, "ppm", nullptr, nullptr, 0},
    {"gas", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"gas_value", ExposeType::Numeric, Access::State, "LEL", nullptr, nullptr, 0},
    {"silence", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"self_test", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_qaxkdgyt_5, 4},
    {"fault", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE204_qaxkdgyt_6, 6},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_qaxkdgyt{
    .zigbee_models=kM__TZE204_qaxkdgyt,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_qaxkdgyt,.manufacturer_names_count=1,
    .model="TS0601__TZE204_qaxkdgyt",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_qaxkdgyt,.exposes_count=sizeof(kAutoExposes__TZE204_qaxkdgyt)/sizeof(kAutoExposes__TZE204_qaxkdgyt[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_qaxkdgyt,
    .from_zigbee_count=sizeof(kFz__TZE204_qaxkdgyt)/sizeof(kFz__TZE204_qaxkdgyt[0]),
    .to_zigbee=kTz__TZE204_qaxkdgyt,
    .to_zigbee_count=sizeof(kTz__TZE204_qaxkdgyt)/sizeof(kTz__TZE204_qaxkdgyt[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
