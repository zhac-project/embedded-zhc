// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Moes BHT-002 — graduated from generated/ to carry DP101.
// Moes BHT series Thermostat
// z2m-source: moes.ts #BHT-002.
// Phase 2 (2026-04-27): wired Tuya DP map from z2m's legacy.fz.moes_thermostat
// switch table (lib/legacy.ts:1876-2010). DP ids:
//   1   state         Bool   → state ("ON"/"OFF")
//   2   tvMode        Bool   → preset ("program"/"hold")
//   16  heatingSetpt  Numeric→ current_heating_setpoint (no scale by default;
//                              `_TZE200_5toc8efa`/`_TZE204_5toc8efa` use /10)
//   18  maxTempLimit  Numeric→ max_temperature_limit
//   19  maxTemp       Numeric→ max_temperature
//   20  deadZoneTemp  Numeric→ deadzone_temperature
//   24  localTemp     Numeric, divisor 10 → local_temperature
//   26  minTempLimit  Numeric→ min_temperature_limit
//   27  tempCalib     Numeric→ local_temperature_calibration
//   36  valve         Bool   → running_state (idle/heat — emitted as bool)
//   40  childLock     Bool   → child_lock (LOCK/UNLOCK)
//   43  sensor        Enum   → sensor (IN/AL/OU)
//   101 moesSchedule  Raw    → program (weekly schedule string; encode+decode
//                              codec lives in tuya/_shared.cpp behind
//                              kTuyaDpFlagMoesSchedule). 3 day-groups × 4
//                              periods "HH:MM/T.t", joined by " | ".
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::moes {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_BHT_002_sensor[] = {
    { 0, "IN" },
    { 1, "AL" },
    { 2, "OU" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_BHT_002[] = {
    { 1,   "state",                          ::zhc::TuyaDpType::Bool,    1,  nullptr, 0, 0 },
    { 2,   "preset",                         ::zhc::TuyaDpType::Bool,    1,  nullptr, 0, 0 },
    { 16,  "current_heating_setpoint",       ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 18,  "max_temperature_limit",          ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 19,  "max_temperature",                ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 20,  "deadzone_temperature",           ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 24,  "local_temperature",              ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 26,  "min_temperature_limit",          ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 27,  "local_temperature_calibration",  ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 36,  "running_state",                  ::zhc::TuyaDpType::Bool,    1,  nullptr, 0, 0 },
    { 40,  "child_lock",                     ::zhc::TuyaDpType::Bool,    1,  nullptr, 0, 0 },
    { 43,  "sensor",                         ::zhc::TuyaDpType::Enum,    1,
           kEnum_BHT_002_sensor, sizeof(kEnum_BHT_002_sensor)/sizeof(kEnum_BHT_002_sensor[0]), 0 },
    { 101, "program",                        ::zhc::TuyaDpType::Raw,     1,  nullptr, 0,
           ::zhc::tuya::kTuyaDpFlagMoesSchedule },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_BHT_002{
    kEntries_BHT_002,
    sizeof(kEntries_BHT_002)/sizeof(kEntries_BHT_002[0])
};

constexpr FzConverter kFzDp_BHT_002{
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
    .user_config       = &kMap_BHT_002,
};
constexpr TzConverter kTzDp_BHT_002{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_BHT_002,
};

const FzConverter* const kFz_BHT_002[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_BHT_002,
};
const TzConverter* const kTz_BHT_002[] = { &kTzDp_BHT_002 };

constexpr const char* kModels_BHT_002[] = { "TS0601" };
constexpr const char* kManus_BHT_002[] = {
    "_TZE200_aoclfnxz", "_TZE200_ztvwu4nk", "_TZE204_5toc8efa", "_TZE200_5toc8efa",
    "_TZE200_ye5jkfsb", "_TZE284_ye5jkfsb", "_TZE204_aoclfnxz", "_TZE200_u9bfwha0",
    "_TZE204_u9bfwha0"
};

constexpr Expose kExp_BHT_002[] = {
    { "current_heating_setpoint",      ExposeType::Numeric, ::zhc::Access::StateSet, "C",     nullptr, nullptr, 0 },
    { "local_temperature",             ExposeType::Numeric, ::zhc::Access::State,    "C",     nullptr, nullptr, 0 },
    { "local_temperature_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, "C",     nullptr, nullptr, 0 },
    { "max_temperature_limit",         ExposeType::Numeric, ::zhc::Access::StateSet, "C",     nullptr, nullptr, 0 },
    { "min_temperature_limit",         ExposeType::Numeric, ::zhc::Access::StateSet, "C",     nullptr, nullptr, 0 },
    { "deadzone_temperature",          ExposeType::Numeric, ::zhc::Access::StateSet, "C",     nullptr, nullptr, 0 },
    { "child_lock",                    ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "preset",                        ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "sensor",                        ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state",                         ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "running_state",                 ExposeType::Binary,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "program",                       ExposeType::String,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_BHT_002[] = { { 1, 0xEF00 } };

}  // namespace

constexpr WhiteLabel kWhiteLabels_BHT_002[] = {
    {"Moes","BHT-002/BHT-006"},
};
extern const PreparedDefinition kDef_BHT_002{
    .zigbee_models=kModels_BHT_002, .zigbee_models_count=sizeof(kModels_BHT_002)/sizeof(kModels_BHT_002[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_BHT_002, .manufacturer_names_count=sizeof(kManus_BHT_002)/sizeof(kManus_BHT_002[0]),
    .model="BHT-002", .vendor="Moes",
    .meta=nullptr, .exposes=kExp_BHT_002, .exposes_count=sizeof(kExp_BHT_002)/sizeof(kExp_BHT_002[0]),
    .white_labels=kWhiteLabels_BHT_002, .white_labels_count=sizeof(kWhiteLabels_BHT_002)/sizeof(kWhiteLabels_BHT_002[0]),
    .from_zigbee=kFz_BHT_002, .from_zigbee_count=sizeof(kFz_BHT_002)/sizeof(kFz_BHT_002[0]),
    .to_zigbee=kTz_BHT_002, .to_zigbee_count=sizeof(kTz_BHT_002)/sizeof(kTz_BHT_002[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBind_BHT_002, .bindings_count=sizeof(kBind_BHT_002)/sizeof(kBind_BHT_002[0]),
};

}  // namespace zhc::devices::moes
