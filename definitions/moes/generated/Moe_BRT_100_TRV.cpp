// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Moes BRT-100-TRV — auto-generated.
// Thermostatic radiator valve
// z2m-source: moes.ts #BRT-100-TRV.
// Phase 2 (2026-04-27): wired Tuya DP map from legacy.fz.moesS_thermostat
// (legacy.ts:2012-2160). DP ids:
//   1   systemMode    Enum   → preset (programming/manual/temporary_manual/holiday)
//   2   heatingSetpt  Numeric→ current_heating_setpoint
//   3   localTemp     Numeric, /10 → local_temperature
//   4   boostHeating  Bool   → boost_heating
//   5   boostCountdown Numeric → boost_heating_countdown
//   7   reset         Bool   → running_state
//   8   windowDetectFunktion Bool → window_detection
//   9   windowDetect  Bool   → window
//   13  childLock     Bool   → child_lock
//   14  battery       Numeric→ battery
//   103 boostCountdownTimeSet Numeric → boost_heating_countdown_time_set
//   104 valvePosition Numeric→ position
//   105 compensationTempSet Numeric → local_temperature_calibration
//   106 ecoMode       Bool   → eco_mode
//   107 ecoModeTempSet Numeric→ eco_temperature
//   108 maxTempSet    Numeric→ max_temperature
//   109 minTempSet    Numeric→ min_temperature
// TODO(Phase 2): DP 101 (moesSschedule) — packed schedule format with 12
// items + per-item program mode; needs hand-rolled raw decoder. Omitted.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::moes {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_BRT_100_preset[] = {
    { 0, "programming" }, { 1, "manual" }, { 2, "temporary_manual" }, { 3, "holiday" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_BRT_100[] = {
    { 1,   "preset",                          ::zhc::TuyaDpType::Enum,    1,
           kEnum_BRT_100_preset, sizeof(kEnum_BRT_100_preset)/sizeof(kEnum_BRT_100_preset[0]), 0 },
    { 2,   "current_heating_setpoint",        ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 3,   "local_temperature",               ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 4,   "boost_heating",                   ::zhc::TuyaDpType::Bool,    1,  nullptr, 0, 0 },
    { 5,   "boost_heating_countdown",         ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 7,   "running_state",                   ::zhc::TuyaDpType::Bool,    1,  nullptr, 0, 0 },
    { 8,   "window_detection",                ::zhc::TuyaDpType::Bool,    1,  nullptr, 0, 0 },
    { 9,   "window",                          ::zhc::TuyaDpType::Bool,    1,  nullptr, 0, 0 },
    { 13,  "child_lock",                      ::zhc::TuyaDpType::Bool,    1,  nullptr, 0, 0 },
    { 14,  "battery",                         ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 103, "boost_heating_countdown_time_set",::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 104, "position",                        ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 105, "local_temperature_calibration",   ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 106, "eco_mode",                        ::zhc::TuyaDpType::Bool,    1,  nullptr, 0, 0 },
    { 107, "eco_temperature",                 ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 108, "max_temperature",                 ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 109, "min_temperature",                 ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
};

constexpr ::zhc::tuya::TuyaDatapointMap kMap_BRT_100{
    kEntries_BRT_100,
    sizeof(kEntries_BRT_100)/sizeof(kEntries_BRT_100[0])
};

constexpr FzConverter kFzDp_BRT_100{
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
    .user_config       = &kMap_BRT_100,
};
constexpr TzConverter kTzDp_BRT_100{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_BRT_100,
};

const FzConverter* const kFz_BRT_100[] = {
    &::zhc::tuya::kFzIgnoreTuyaSetTime,
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_BRT_100,
};
const TzConverter* const kTz_BRT_100[] = { &kTzDp_BRT_100 };

constexpr const char* kModels_BRT_100[] = { "TS0601" };
constexpr const char* kManus_BRT_100[] = { "_TZE200_b6wax7g0", "_TZE200_qsoecqlk", "_TZE200_6y7kyjga" };

constexpr Expose kExp_BRT_100[] = {
    { "current_heating_setpoint",      ExposeType::Numeric, ::zhc::Access::StateSet, "C",     nullptr, nullptr, 0 },
    { "local_temperature",             ExposeType::Numeric, ::zhc::Access::State,    "C",     nullptr, nullptr, 0 },
    { "local_temperature_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, "C",     nullptr, nullptr, 0 },
    { "max_temperature",               ExposeType::Numeric, ::zhc::Access::StateSet, "C",     nullptr, nullptr, 0 },
    { "min_temperature",               ExposeType::Numeric, ::zhc::Access::StateSet, "C",     nullptr, nullptr, 0 },
    { "eco_temperature",               ExposeType::Numeric, ::zhc::Access::StateSet, "C",     nullptr, nullptr, 0 },
    { "battery",                       ExposeType::Numeric, ::zhc::Access::State,    "%",     nullptr, nullptr, 0 },
    { "position",                      ExposeType::Numeric, ::zhc::Access::State,    "%",     nullptr, nullptr, 0 },
    { "preset",                        ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "child_lock",                    ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "window_detection",              ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "window",                        ExposeType::Binary,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "boost_heating",                 ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "boost_heating_countdown",       ExposeType::Numeric, ::zhc::Access::State,    "s",     nullptr, nullptr, 0 },
    { "boost_heating_countdown_time_set",ExposeType::Numeric, ::zhc::Access::StateSet, "s",   nullptr, nullptr, 0 },
    { "eco_mode",                      ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "running_state",                 ExposeType::Binary,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_BRT_100[] = { {1, 0xEF00} };
}  // namespace

constexpr WhiteLabel kWhiteLabels_BRT_100_TRV[] = {
    {"Sibling","Powerswitch-ZK(W)"},
};
extern const PreparedDefinition kDef_BRT_100_TRV{
    .zigbee_models=kModels_BRT_100, .zigbee_models_count=sizeof(kModels_BRT_100)/sizeof(kModels_BRT_100[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_BRT_100, .manufacturer_names_count=sizeof(kManus_BRT_100)/sizeof(kManus_BRT_100[0]),
    .model="BRT-100-TRV", .vendor="Moes",
    .meta=nullptr, .exposes=kExp_BRT_100, .exposes_count=sizeof(kExp_BRT_100)/sizeof(kExp_BRT_100[0]),
    .white_labels=kWhiteLabels_BRT_100_TRV, .white_labels_count=sizeof(kWhiteLabels_BRT_100_TRV)/sizeof(kWhiteLabels_BRT_100_TRV[0]),
    .from_zigbee=kFz_BRT_100, .from_zigbee_count=sizeof(kFz_BRT_100)/sizeof(kFz_BRT_100[0]),
    .to_zigbee=kTz_BRT_100, .to_zigbee_count=sizeof(kTz_BRT_100)/sizeof(kTz_BRT_100[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBind_BRT_100, .bindings_count=sizeof(kBind_BRT_100)/sizeof(kBind_BRT_100[0]),
};
}  // namespace zhc::devices::moes
