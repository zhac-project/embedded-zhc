// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_5m4nchbm).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_5m4nchbm[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 20, "voltage", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 18, "current", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 19, "power", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 17, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 47, "temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 53, "leakage_current", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 41, "leakage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 42, "over_voltage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 43, "under_voltage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 44, "over_current_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 45, "temp_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "fault_code", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_5m4nchbm{ kEntries__TZE284_5m4nchbm, 13 };
constexpr FzConverter kFzDp__TZE284_5m4nchbm{
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
    .user_config       = &kMap__TZE284_5m4nchbm,
};
const FzConverter* const kFz__TZE284_5m4nchbm[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_5m4nchbm,
};
constexpr TzConverter kTzDp__TZE284_5m4nchbm{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_5m4nchbm,
};
const TzConverter* const kTz__TZE284_5m4nchbm[] = { &kTzDp__TZE284_5m4nchbm };
constexpr const char* kM__TZE284_5m4nchbm[] = { "TS0601" };
constexpr const char* kN__TZE284_5m4nchbm[] = { "_TZE284_5m4nchbm" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr Expose kAutoExposes__TZE284_5m4nchbm[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"leakage_current", ExposeType::Numeric, Access::State, "mA", nullptr, nullptr, 0, ExposeCategory::Diagnostic},
    {"over_voltage_threshold", ExposeType::Numeric, Access::StateSet, "V", nullptr, nullptr, 0, ExposeCategory::State, 220, 265, 0},
    {"under_voltage_threshold", ExposeType::Numeric, Access::StateSet, "V", nullptr, nullptr, 0, ExposeCategory::State, 76, 240, 0},
    {"over_current_threshold", ExposeType::Numeric, Access::StateSet, "A", nullptr, nullptr, 0, ExposeCategory::State, 1, 63, 0},
    {"leakage_threshold", ExposeType::Numeric, Access::StateSet, "mA", nullptr, nullptr, 0, ExposeCategory::State, 10, 100, 0},
    {"temp_threshold", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 40, 150, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_5m4nchbm{
    .zigbee_models=kM__TZE284_5m4nchbm,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_5m4nchbm,.manufacturer_names_count=1,
    .model="TS0601__TZE284_5m4nchbm",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_5m4nchbm,.exposes_count=sizeof(kAutoExposes__TZE284_5m4nchbm)/sizeof(kAutoExposes__TZE284_5m4nchbm[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_5m4nchbm,
    .from_zigbee_count=sizeof(kFz__TZE284_5m4nchbm)/sizeof(kFz__TZE284_5m4nchbm[0]),
    .to_zigbee=kTz__TZE284_5m4nchbm,
    .to_zigbee_count=sizeof(kTz__TZE284_5m4nchbm)/sizeof(kTz__TZE284_5m4nchbm[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
