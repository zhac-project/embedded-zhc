// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_ny94onlb).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_ny94onlb[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 23, "produced_energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 29, "power", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 32, "ac_frequency", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 50, "power_factor", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "voltage_a", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 103, "current_a", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 104, "power_a", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "voltage_b", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 106, "current_b", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 107, "power_b", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 108, "voltage_c", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 109, "current_c", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 110, "power_c", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_ny94onlb{ kEntries__TZE204_ny94onlb, 14 };
constexpr FzConverter kFzDp__TZE204_ny94onlb{
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
    .user_config       = &kMap__TZE204_ny94onlb,
};
const FzConverter* const kFz__TZE204_ny94onlb[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_ny94onlb,
};
constexpr TzConverter kTzDp__TZE204_ny94onlb{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_ny94onlb,
};
const TzConverter* const kTz__TZE204_ny94onlb[] = { &kTzDp__TZE204_ny94onlb };
constexpr const char* kM__TZE204_ny94onlb[] = { "TS0601" };
constexpr const char* kN__TZE204_ny94onlb[] = { "_TZE204_ny94onlb" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr Expose kAutoExposes__TZE204_ny94onlb[] = {
    {"voltage_a", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"voltage_b", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"voltage_c", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"power_a", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_b", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_c", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"current_a", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_b", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_c", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"produced_energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power_factor", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"ac_frequency", ExposeType::Numeric, Access::State, "Hz", nullptr, nullptr, 0},
    {"data_report_duration", ExposeType::Numeric, Access::Set, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 30, 3600, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_ny94onlb{
    .zigbee_models=kM__TZE204_ny94onlb,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_ny94onlb,.manufacturer_names_count=1,
    .model="TS0601__TZE204_ny94onlb",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_ny94onlb,.exposes_count=sizeof(kAutoExposes__TZE204_ny94onlb)/sizeof(kAutoExposes__TZE204_ny94onlb[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_ny94onlb,
    .from_zigbee_count=sizeof(kFz__TZE204_ny94onlb)/sizeof(kFz__TZE204_ny94onlb[0]),
    .to_zigbee=kTz__TZE204_ny94onlb,
    .to_zigbee_count=sizeof(kTz__TZE204_ny94onlb)/sizeof(kTz__TZE204_ny94onlb[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
