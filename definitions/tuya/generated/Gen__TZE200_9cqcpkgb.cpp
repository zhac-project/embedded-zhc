// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_9cqcpkgb).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_9cqcpkgb_temperature_unit_dp9[] = {
    { 0, "celsius" },
    { 1, "fahrenheit" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_9cqcpkgb_battery_state_dp14[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_9cqcpkgb[] = {
    { 3, "soil_moisture", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "temperature_unit", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_9cqcpkgb_temperature_unit_dp9, sizeof(kEnum__TZE200_9cqcpkgb_temperature_unit_dp9)/sizeof(kEnum__TZE200_9cqcpkgb_temperature_unit_dp9[0]) },
    { 14, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_9cqcpkgb_battery_state_dp14, sizeof(kEnum__TZE200_9cqcpkgb_battery_state_dp14)/sizeof(kEnum__TZE200_9cqcpkgb_battery_state_dp14[0]) },
    { 15, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_9cqcpkgb{ kEntries__TZE200_9cqcpkgb, 5 };
constexpr FzConverter kFzDp__TZE200_9cqcpkgb{
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
    .user_config       = &kMap__TZE200_9cqcpkgb,
};
const FzConverter* const kFz__TZE200_9cqcpkgb[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_9cqcpkgb,
};
constexpr TzConverter kTzDp__TZE200_9cqcpkgb{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_9cqcpkgb,
};
const TzConverter* const kTz__TZE200_9cqcpkgb[] = { &kTzDp__TZE200_9cqcpkgb };
constexpr const char* kM__TZE200_9cqcpkgb[] = { "TS0601" };
constexpr const char* kN__TZE200_9cqcpkgb[] = { "_TZE200_9cqcpkgb" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_9cqcpkgb_2[] = {"celsius", "fahrenheit"};
constexpr const char* kAutoOpts__TZE200_9cqcpkgb_4[] = {"low", "medium", "high"};
constexpr Expose kAutoExposes__TZE200_9cqcpkgb[] = {
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"soil_moisture", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"temperature_unit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_9cqcpkgb_2, 2},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
    {"battery_state", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_9cqcpkgb_4, 3, ExposeCategory::Diagnostic},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_9cqcpkgb{
    .zigbee_models=kM__TZE200_9cqcpkgb,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_9cqcpkgb,.manufacturer_names_count=1,
    .model="TS0601__TZE200_9cqcpkgb",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_9cqcpkgb,.exposes_count=sizeof(kAutoExposes__TZE200_9cqcpkgb)/sizeof(kAutoExposes__TZE200_9cqcpkgb[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_9cqcpkgb,
    .from_zigbee_count=sizeof(kFz__TZE200_9cqcpkgb)/sizeof(kFz__TZE200_9cqcpkgb[0]),
    .to_zigbee=kTz__TZE200_9cqcpkgb,
    .to_zigbee_count=sizeof(kTz__TZE200_9cqcpkgb)/sizeof(kTz__TZE200_9cqcpkgb[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
