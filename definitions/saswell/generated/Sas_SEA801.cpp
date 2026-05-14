// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Saswell SEA801-Zigbee/SEA802-Zigbee — Tuya DP-mapped TRV.
// Thermostatic radiator valve (Tuya OEM, fingerprint TS0601 group).
// z2m-source: saswell.ts #SEA801-Zigbee/SEA802-Zigbee.
//
// DP map derived from legacy.fz.saswell_thermostat + legacy.tz.saswell_*:
//   3  heating        bool   → heating (valve open/closed)
//   8  window_det.    bool   → window_detection
//  10  frost_det.     bool   → frost_detection
//  27  temp cal       s32    → local_temperature_calibration (±6)
//  40  child_lock     bool   → child_lock
// 101  state          bool   → system_mode (on=heat/off=off)
// 102  local_temp     s32/10 → local_temperature
// 103  heat_setpoint  s32/10 → current_heating_setpoint (5–30 °C)
// 104  valve_pos      s32    → valve_position (0–100 %)
// 105  battery_low    bool   → battery_low
// 106  away_mode      bool   → away_mode
// 107  schedule_mode  enum   → schedule_mode
// 130  anti_scaling   bool   → anti_scaling
//
// Covers the TS0601/_TZE200/_TZE284 fingerprint group (16 manu names).
// Legacy `_TYST11_*` fingerprints (modelIDs with embedded NUL) aren't
// representable as `const char*` zigbee_models and are omitted.

#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::saswell {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_SEA801[] = {
    {   3, "heating",                       ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
    {   8, "window_detection",              ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
    {  10, "frost_detection",               ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
    {  27, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    {  40, "child_lock",                    ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
    { 101, "system_mode",                   ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
    { 102, "local_temperature",             ::zhc::TuyaDpType::Numeric,10, nullptr, 0, 0 },
    { 103, "current_heating_setpoint",      ::zhc::TuyaDpType::Numeric,10, nullptr, 0, 0 },
    { 104, "valve_position",                ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "battery_low",                   ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
    { 106, "away_mode",                     ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
    { 107, "schedule_mode",                 ::zhc::TuyaDpType::Enum,    1, nullptr, 0, 0 },
    { 130, "anti_scaling",                  ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_SEA801{
    kEntries_SEA801,
    sizeof(kEntries_SEA801) / sizeof(kEntries_SEA801[0]),
};

constexpr FzConverter kFzDp_SEA801{
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
    .user_config       = &kMap_SEA801,
};
const FzConverter* const kFz_SEA801[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_SEA801,
};

constexpr TzConverter kTzDp_SEA801{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_SEA801,
};
const TzConverter* const kTz_SEA801[] = { &kTzDp_SEA801 };

constexpr const char* kModels_SEA801[] = { "TS0601" };
constexpr const char* kManus_SEA801[]  = {
    "_TZE200_c88teujp", "_TZE200_yw7cahqs", "_TZE200_azqp6ssj",
    "_TZE200_zuhszj9s", "_TZE200_9gvruqf5", "_TZE200_zr9c0day",
    "_TZE200_0dvm9mva", "_TZE284_0dvm9mva", "_TZE200_h4cgnbzg",
    "_TZE200_gd4rvykv", "_TZE200_exfrnlow", "_TZE200_9m4kmbfu",
    "_TZE284_9m4kmbfu", "_TZE200_3yp57tby", "_TZE200_7p8ugv8d",
    "_TZE284_3yp57tby",
};

constexpr Expose kExposes_SEA801[] = {
    { "battery_low",                    ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0 },
    { "window_detection",               ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "child_lock",                     ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "away_mode",                      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "frost_detection",                ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "anti_scaling",                   ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "heating",                        ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0 },
    { "system_mode",                    ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "local_temperature",              ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0 },
    { "current_heating_setpoint",       ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "local_temperature_calibration",  ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "valve_position",                 ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0 },
    { "schedule_mode",                  ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_SEA801[] = {
    { 1, 0xEF00 },
};

}  // namespace


constexpr WhiteLabel kWhiteLabels_SEA801[] = {
    {"HiHome","WZB-TRVL"},
    {"Hama","00176592"},
    {"Maginon","WT-1"},
    {"RTX","ZB-RT1"},
    {"SETTI+","TRV001"},
    {"Royal Thermo","RTE 77.001B"},
};
extern const PreparedDefinition kDef_SEA801{
    .zigbee_models=kModels_SEA801, .zigbee_models_count=sizeof(kModels_SEA801)/sizeof(kModels_SEA801[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SEA801,
    .manufacturer_names_count=sizeof(kManus_SEA801)/sizeof(kManus_SEA801[0]),
    .model="SEA801-Zigbee/SEA802-Zigbee", .vendor="Saswell",
    .meta=nullptr,
    .exposes=kExposes_SEA801, .exposes_count=sizeof(kExposes_SEA801)/sizeof(kExposes_SEA801[0]),
    .white_labels=kWhiteLabels_SEA801, .white_labels_count=sizeof(kWhiteLabels_SEA801)/sizeof(kWhiteLabels_SEA801[0]),
    .from_zigbee=kFz_SEA801, .from_zigbee_count=sizeof(kFz_SEA801)/sizeof(kFz_SEA801[0]),
    .to_zigbee=kTz_SEA801,   .to_zigbee_count=sizeof(kTz_SEA801)/sizeof(kTz_SEA801[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBindings_SEA801, .bindings_count=sizeof(kBindings_SEA801)/sizeof(kBindings_SEA801[0]),
};

}  // namespace zhc::devices::saswell
