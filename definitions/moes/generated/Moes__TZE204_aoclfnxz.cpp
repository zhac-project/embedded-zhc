// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/moes.ts (fingerprint TS0601 / _TZE204_aoclfnxz).
// Phase 2 (2026-04-27): wired Tuya DP map from legacy.fz.moes_thermostat
// (legacy.ts:1876-2010). See Moe_BHT_002.cpp for full DP table notes.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::moes {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_BHT_002_sensor[] = {
    { 0, "IN" }, { 1, "AL" }, { 2, "OU" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_BHT_002[] = {
    { 1,  "state",                          ::zhc::TuyaDpType::Bool,    1,  nullptr, 0, 0 },
    { 2,  "preset",                         ::zhc::TuyaDpType::Bool,    1,  nullptr, 0, 0 },
    { 16, "current_heating_setpoint",       ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 18, "max_temperature_limit",          ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 19, "max_temperature",                ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 20, "deadzone_temperature",           ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 24, "local_temperature",              ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 26, "min_temperature_limit",          ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 27, "local_temperature_calibration",  ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    { 36, "running_state",                  ::zhc::TuyaDpType::Bool,    1,  nullptr, 0, 0 },
    { 40, "child_lock",                     ::zhc::TuyaDpType::Bool,    1,  nullptr, 0, 0 },
    { 43, "sensor",                         ::zhc::TuyaDpType::Enum,    1,
           kEnum_BHT_002_sensor, sizeof(kEnum_BHT_002_sensor)/sizeof(kEnum_BHT_002_sensor[0]), 0 },
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

constexpr const char* kM_BHT_002[] = { "TS0601" };
constexpr const char* kN_BHT_002[] = { "_TZE204_aoclfnxz" };


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
};

constexpr BindingSpec kBindings_BHT_002[] = { {1, 0xEF00} };
}  // namespace
extern const PreparedDefinition kDefMoes__TZE204_aoclfnxz{
    .zigbee_models=kM_BHT_002,.zigbee_models_count=sizeof(kM_BHT_002)/sizeof(kM_BHT_002[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_BHT_002, .manufacturer_names_count=1,
    .model="TS0601__TZE204_aoclfnxz", .vendor="Moes",
    .meta=nullptr, .exposes=kExp_BHT_002, .exposes_count=sizeof(kExp_BHT_002)/sizeof(kExp_BHT_002[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_BHT_002, .from_zigbee_count=sizeof(kFz_BHT_002)/sizeof(kFz_BHT_002[0]),
    .to_zigbee=kTz_BHT_002, .to_zigbee_count=sizeof(kTz_BHT_002)/sizeof(kTz_BHT_002[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBindings_BHT_002, .bindings_count=sizeof(kBindings_BHT_002)/sizeof(kBindings_BHT_002[0]),
};
}  // namespace zhc::devices::moes
