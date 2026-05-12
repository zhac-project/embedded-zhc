// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Netica FRZ1 — hand-rewritten 2026-04-29d (was misaligned
// on/off+battery stub; z2m defines a thermostat with T+H + 3
// manuSpec attrs).
// FreezBee, a smart thermostat designed to operate with Frisquet boilers.
// z2m-source: netica.ts #FRZ1.
//
// z2m manuSpec hvacThermostat attrs (mfg = ManufacturerCode.RESERVED_10
// = 0xFFFF):
//   0x4000 INT16 remoteTemperature      (×100 °C)
//   0x4001 BOOL  useRemoteTemperature
//   0x4002 INT16 targetWaterTemperature (×100 °C)
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::netica {
namespace {

constexpr const char* kModels_FRZ1[] = { "FRZ1" };

constexpr std::uint16_t kNeticaMfgCode = 0xFFFF;  // ManufacturerCode.RESERVED_10

constexpr ::zhc::generic::ZclWriteSpec kSpecRemoteTemperature{
    "remote_temperature",      0x4000, 0x29 /* INT16 */, kNeticaMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecUseRemoteTemperature{
    "use_remote_temperature",  0x4001, 0x10 /* BOOL */,  kNeticaMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecTargetWaterTemperature{
    "target_water_temperature",0x4002, 0x29 /* INT16 */, kNeticaMfgCode, nullptr, 0,
};

constexpr ::zhc::TzConverter kTzRemoteTemperature{
    .key="remote_temperature", .cluster="hvacThermostat",
    .cluster_id=0x0201, .command_id=0x02,
    .fn=&::zhc::generic::tz_zcl_write_attr,
    .user_config=&kSpecRemoteTemperature,
};
constexpr ::zhc::TzConverter kTzUseRemoteTemperature{
    .key="use_remote_temperature", .cluster="hvacThermostat",
    .cluster_id=0x0201, .command_id=0x02,
    .fn=&::zhc::generic::tz_zcl_write_attr,
    .user_config=&kSpecUseRemoteTemperature,
};
constexpr ::zhc::TzConverter kTzTargetWaterTemperature{
    .key="target_water_temperature", .cluster="hvacThermostat",
    .cluster_id=0x0201, .command_id=0x02,
    .fn=&::zhc::generic::tz_zcl_write_attr,
    .user_config=&kSpecTargetWaterTemperature,
};

const FzConverter* const kFz_FRZ1[] = {
    &::zhc::generic::kFzThermostat,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_FRZ1[] = {
    &::zhc::generic::kTzThermostat,
    &::zhc::generic::kTzMinHeatSetpointLimit,
    &::zhc::generic::kTzMaxHeatSetpointLimit,
    &::zhc::generic::kTzSetpointRaiseLower,
    &kTzRemoteTemperature,
    &kTzUseRemoteTemperature,
    &kTzTargetWaterTemperature,
};

constexpr Expose kExp_FRZ1[] = {
    { "local_temperature",          ExposeType::Numeric, ::zhc::Access::State,    "C", nullptr, nullptr, 0 },
    { "current_heating_setpoint",   ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
    { "system_mode",                ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "min_heat_setpoint_limit",    ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
    { "max_heat_setpoint_limit",    ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
    { "temperature",                ExposeType::Numeric, ::zhc::Access::State,    "C", nullptr, nullptr, 0 },
    { "humidity",                   ExposeType::Numeric, ::zhc::Access::State,    "%", nullptr, nullptr, 0 },
    { "battery",                    ExposeType::Numeric, ::zhc::Access::State,    "%", nullptr, nullptr, 0 },
    { "voltage",                    ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
    { "remote_temperature",         ExposeType::Numeric, ::zhc::Access::Set,      "C", nullptr, nullptr, 0 },
    { "use_remote_temperature",     ExposeType::Binary,  ::zhc::Access::Set,      nullptr, nullptr, nullptr, 0 },
    { "target_water_temperature",   ExposeType::Numeric, ::zhc::Access::Set,      "C", nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_FRZ1[] = {
    { 1, 0x0001 },   // genPowerCfg
    { 1, 0x0201 },   // hvacThermostat
    { 1, 0x0402 },   // msTemperatureMeasurement
    { 1, 0x0405 },   // msRelativeHumidity
};

}  // namespace

extern const PreparedDefinition kDef_FRZ1{
    .zigbee_models=kModels_FRZ1, .zigbee_models_count=sizeof(kModels_FRZ1)/sizeof(kModels_FRZ1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="FRZ1", .vendor="Netica",
    .meta=nullptr,
    .exposes=kExp_FRZ1, .exposes_count=sizeof(kExp_FRZ1)/sizeof(kExp_FRZ1[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_FRZ1, .from_zigbee_count=sizeof(kFz_FRZ1)/sizeof(kFz_FRZ1[0]),
    .to_zigbee=kTz_FRZ1,   .to_zigbee_count=sizeof(kTz_FRZ1)/sizeof(kTz_FRZ1[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_FRZ1, .bindings_count=sizeof(kBind_FRZ1)/sizeof(kBind_FRZ1[0]),
};

}  // namespace zhc::devices::netica
