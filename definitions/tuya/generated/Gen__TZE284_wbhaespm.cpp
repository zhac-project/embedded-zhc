// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_wbhaespm).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_wbhaespm[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 16, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 17, "overload_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "overload_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "leakage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "leakage_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "high_temperature_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 17, "high_temperature_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "over_current_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "over_current_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "over_voltage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "over_voltage_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "under_voltage_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "under_voltage_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "insufficient_balance_threshold", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 18, "insufficient_balance_breaker", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 21, "leakage_test", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_wbhaespm{ kEntries__TZE284_wbhaespm, 18 };
constexpr FzConverter kFzDp__TZE284_wbhaespm{
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
    .user_config       = &kMap__TZE284_wbhaespm,
};
const FzConverter* const kFz__TZE284_wbhaespm[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_wbhaespm,
};
constexpr TzConverter kTzDp__TZE284_wbhaespm{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_wbhaespm,
};
const TzConverter* const kTz__TZE284_wbhaespm[] = { &kTzDp__TZE284_wbhaespm };
constexpr const char* kM__TZE284_wbhaespm[] = { "TS0601" };
constexpr const char* kN__TZE284_wbhaespm[] = { "_TZE284_wbhaespm" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr Expose kAutoExposes__TZE284_wbhaespm[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"voltage_a", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"voltage_b", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"voltage_c", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"power_a", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_b", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_c", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"current_a", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_b", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_c", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"leakage_test", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"over_current_breaker", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"over_current_threshold", ExposeType::Numeric, Access::StateSet, "A", nullptr, nullptr, 0, ExposeCategory::State, 1, 63, 0},
    {"over_voltage_breaker", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"over_voltage_threshold", ExposeType::Numeric, Access::StateSet, "V", nullptr, nullptr, 0, ExposeCategory::State, 250, 300, 0},
    {"under_voltage_breaker", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"under_voltage_threshold", ExposeType::Numeric, Access::StateSet, "V", nullptr, nullptr, 0, ExposeCategory::State, 150, 200, 0},
    {"insufficient_balance_breaker", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"insufficient_balance_threshold", ExposeType::Numeric, Access::StateSet, "kWh", nullptr, nullptr, 0, ExposeCategory::State, 1, 65535, 0},
    {"overload_breaker", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"overload_threshold", ExposeType::Numeric, Access::StateSet, "kW", nullptr, nullptr, 0, ExposeCategory::State, 1, 25, 0},
    {"leakage_breaker", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"leakage_threshold", ExposeType::Numeric, Access::StateSet, "mA", nullptr, nullptr, 0, ExposeCategory::State, 10, 90, 0},
    {"high_temperature_breaker", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"high_temperature_threshold", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 40, 100, 0},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE284_wbhaespm[] = {
    {"SUTON","STB3L-125/ZJ"},
};
extern const PreparedDefinition kDefGen__TZE284_wbhaespm{
    .zigbee_models=kM__TZE284_wbhaespm,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_wbhaespm,.manufacturer_names_count=1,
    .model="TS0601__TZE284_wbhaespm",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_wbhaespm,.exposes_count=sizeof(kAutoExposes__TZE284_wbhaespm)/sizeof(kAutoExposes__TZE284_wbhaespm[0]),
    .white_labels=kWhiteLabels_Gen_TZE284_wbhaespm, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE284_wbhaespm)/sizeof(kWhiteLabels_Gen_TZE284_wbhaespm[0]),
    .from_zigbee=kFz__TZE284_wbhaespm,
    .from_zigbee_count=sizeof(kFz__TZE284_wbhaespm)/sizeof(kFz__TZE284_wbhaespm[0]),
    .to_zigbee=kTz__TZE284_wbhaespm,
    .to_zigbee_count=sizeof(kTz__TZE284_wbhaespm)/sizeof(kTz__TZE284_wbhaespm[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
