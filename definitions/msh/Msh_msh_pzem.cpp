// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Msh msh.pzem — 100 A AC DIN power meter (z2m v26.76.0 parity).
// MSH 100A AC DIN Power Meter (PZEM, PTVO firmware)
// z2m-source: msh.ts #msh.pzem
//   extend: [
//     m.deviceEndpoints({endpoints:{l1:1, l2:2, l3:3}}),
//     m.electricityMeter({endpointNames:["l2"], electricalMeasurementType:"ac",
//                         energy:{divisor:1}, acFrequency:true, powerFactor:true,
//                         configureReporting:false}),
//     m.temperature({endpointNames:["l1"], label:"CPU Temperature"}),
//     m.numeric({name:"l3", attribute:"presentValue", cluster:"genAnalogInput",
//                endpointNames:["l3"], label:"Uptime", unit:"s"}),
//   ]
//
// AC electricity meter → standard clusters, all decodable by the generic
// converters:
//   * power / voltage / current — fz.electrical_measurement on
//     haElectricalMeasurement (0x0B04) endpoint 2. Wired kFzElectricalMeasurement
//     (ActivePower 0x050B, RMSVoltage 0x0505, RMSCurrent 0x0508).
//   * energy — fz.metering on seMetering (0x0702) endpoint 2. Wired kFzMetering
//     (CurrentSummDelivered 0x0000).
//   * CPU temperature — fz.temperature on msTemperatureMeasurement (0x0402, /100)
//     endpoint 1 (z2m endpointNames:["l1"]). Wired kFzTemperature.
// endpoint_map {l1->1, l2->2} suffixes the per-endpoint keys exactly as z2m
// publishes them (temperature_l1, power_l2, current_l2, energy_l2). NOTE: the
// generic dispatch treats `voltage` as a device-global key (battery voltage),
// so it stays bare "voltage" rather than "voltage_l2" — a known cross-device
// suffix quirk, not specific to this def.
//
// DEFERRED (INFRA): ac_frequency (haElectricalMeasurement 0x0300) and
// power_factor (0x0510) have no generic converter yet; the `l3` uptime rides
// genAnalogInput 0x000C presentValue, for which no generic converter exists
// (same infra gap as msh.AirQMon). Not half-built here.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::msh {
namespace {

constexpr const char* kModels_msh_pzem[] = { "msh.pzem" };

const FzConverter* const kFz_msh_pzem[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzMetering,
};

// z2m endpoints {l1:1, l2:2, l3:3}: CPU temp on l1, electricity meter on l2.
// Suffixes keys to temperature_l1 / power_l2 / current_l2 / energy_l2.
constexpr ::zhc::EndpointLabel kEndpoints_msh_pzem[] = { {"l1", 1}, {"l2", 2} };

constexpr Expose kExp_msh_pzem[] = {
    { "temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "power",       ExposeType::Numeric, Access::State, "W",           nullptr, nullptr, 0 },
    { "current",     ExposeType::Numeric, Access::State, "A",           nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, Access::State, "V",           nullptr, nullptr, 0 },
    { "energy",      ExposeType::Numeric, Access::State, "kWh",         nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_msh_pzem[] = {
    { 1, 0x0402 },   // msTemperatureMeasurement (CPU temp, l1)
    { 2, 0x0B04 },   // haElectricalMeasurement (power/voltage/current, l2)
    { 2, 0x0702 },   // seMetering (energy, l2)
};

}  // namespace

extern const PreparedDefinition kDef_msh_pzem{
    .zigbee_models=kModels_msh_pzem, .zigbee_models_count=sizeof(kModels_msh_pzem)/sizeof(kModels_msh_pzem[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="msh.pzem", .vendor="Msh",
    .meta=nullptr, .exposes=kExp_msh_pzem, .exposes_count=sizeof(kExp_msh_pzem)/sizeof(kExp_msh_pzem[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_msh_pzem, .from_zigbee_count=sizeof(kFz_msh_pzem)/sizeof(kFz_msh_pzem[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_msh_pzem,
    .bindings_count           = sizeof(kBind_msh_pzem)/sizeof(kBind_msh_pzem[0]),
    .endpoint_map             = kEndpoints_msh_pzem,
    .endpoint_map_count       = sizeof(kEndpoints_msh_pzem)/sizeof(kEndpoints_msh_pzem[0]),
};

}  // namespace zhc::devices::msh
