// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_jt50ea5d).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_jt50ea5d_report_period_dp4[] = {
    { 0, "1h" },
    { 1, "2h" },
    { 2, "3h" },
    { 3, "4h" },
    { 4, "6h" },
    { 5, "8h" },
    { 6, "12h" },
    { 7, "24h" },
    { 8, "48h" },
    { 9, "72h" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_jt50ea5d[] = {
    { 1, "water_consumed", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 2, "monthly_water_consumption", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "daily_water_consumption", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "report_period", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_jt50ea5d_report_period_dp4, sizeof(kEnum__TZE200_jt50ea5d_report_period_dp4)/sizeof(kEnum__TZE200_jt50ea5d_report_period_dp4[0]) },
    { 6, "prepayment_switch", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 7, "cumulative_heat", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 16, "meter_id", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 21, "inlet_water_temperature", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 22, "outlet_water_temperature", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 24, "battery_voltage", ::zhc::TuyaDpType::Numeric, -10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_jt50ea5d{ kEntries__TZE200_jt50ea5d, 10 };
constexpr FzConverter kFzDp__TZE200_jt50ea5d{
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
    .user_config       = &kMap__TZE200_jt50ea5d,
};
const FzConverter* const kFz__TZE200_jt50ea5d[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_jt50ea5d,
};
constexpr TzConverter kTzDp__TZE200_jt50ea5d{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_jt50ea5d,
};
const TzConverter* const kTz__TZE200_jt50ea5d[] = { &kTzDp__TZE200_jt50ea5d };
constexpr const char* kM__TZE200_jt50ea5d[] = { "TS0601" };
constexpr const char* kN__TZE200_jt50ea5d[] = { "_TZE200_jt50ea5d" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_jt50ea5d_3[] = {"1h", "2h", "3h", "4h", "6h", "8h", "12h", "24h", "48h", "72h"};
constexpr Expose kAutoExposes__TZE200_jt50ea5d[] = {
    {"water_consumed", ExposeType::Numeric, Access::State, "m³", nullptr, nullptr, 0},
    {"monthly_water_consumption", ExposeType::Numeric, Access::State, "m³", nullptr, nullptr, 0},
    {"daily_water_consumption", ExposeType::Numeric, Access::State, "m³", nullptr, nullptr, 0},
    {"report_period", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_jt50ea5d_3, 10},
    {"fault", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
    {"prepayment_switch", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"cumulative_heat", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"meter_id", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
    {"instantaneous_flow_rate", ExposeType::Numeric, Access::State, "m³/h", nullptr, nullptr, 0},
    {"inlet_water_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"outlet_water_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0, ExposeCategory::Diagnostic},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_jt50ea5d{
    .zigbee_models=kM__TZE200_jt50ea5d,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_jt50ea5d,.manufacturer_names_count=1,
    .model="TS0601__TZE200_jt50ea5d",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_jt50ea5d,.exposes_count=sizeof(kAutoExposes__TZE200_jt50ea5d)/sizeof(kAutoExposes__TZE200_jt50ea5d[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_jt50ea5d,
    .from_zigbee_count=sizeof(kFz__TZE200_jt50ea5d)/sizeof(kFz__TZE200_jt50ea5d[0]),
    .to_zigbee=kTz__TZE200_jt50ea5d,
    .to_zigbee_count=sizeof(kTz__TZE200_jt50ea5d)/sizeof(kTz__TZE200_jt50ea5d[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
