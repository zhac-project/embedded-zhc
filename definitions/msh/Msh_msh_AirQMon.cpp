// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Msh msh.AirQMon — air-quality channels restored, wrong class fixed.
// MSH Air quality sensor (PTVO firmware)
// z2m-source: msh.ts #msh.AirQMon
//   fromZigbee: [fz.ptvo_switch_uart, fz.co2, fz.ptvo_switch_analog_input,
//                fz.temperature]
//   exposes: action(UART text), calibrate(l2 enum), co2(),
//            l2(Error), cpu_temperature->temperature@l3, l4(Uptime)
//   endpoint: {l2:2, action:1, l3:3, l4:4}
//
// Bug fixed: the Tier-1 port mis-ported this air-quality sensor as a bare
// on/off switch + phantom battery (genOnOff 0x0006 + genPowerCfg 0x0001) —
// none of which z2m wires. The real headline channels are:
//   * co2  — fz.co2 on the standard msCO2 cluster (0x040D, measuredValue is
//            a single-precision float; ppm = floor(value * 1e6)). Wired the
//            generic kFzCO2.
//   * temperature — fz.temperature on msTemperatureMeasurement (0x0402,
//            /100), reported on endpoint 3 (z2m withEndpoint("l3")). Wired
//            the generic kFzTemperature + endpoint_map {l3->3} so the key
//            suffixes to `temperature_l3` exactly as z2m publishes it.
//
// DEFERRED (INFRA): the PTVO UART `action` (fz.ptvo_switch_uart), the
// `l2`/`l4`/quality analog channels (fz.ptvo_switch_analog_input reads
// genAnalogInput 0x000C presentValue) and the `calibrate` write
// (tz writes a genAnalogInput float) all ride genAnalogInput, for which no
// generic converter exists yet. Not half-built here.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::msh {
namespace {

constexpr const char* kModels_msh_AirQMon[] = { "msh.AirQMon" };

const FzConverter* const kFz_msh_AirQMon[] = {
    &::zhc::generic::kFzCO2,
    &::zhc::generic::kFzTemperature,
};

// z2m withEndpoint("l3") for temperature → endpoint 3 → key `temperature_l3`.
constexpr ::zhc::EndpointLabel kEndpoints_msh_AirQMon[] = { {"l3", 3} };

constexpr Expose kExp_msh_AirQMon[] = {
    { "co2",         ExposeType::Numeric, ::zhc::Access::State, "ppm", nullptr, nullptr, 0 },
    { "temperature", ExposeType::Numeric, ::zhc::Access::State, "°C",  nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_msh_AirQMon[] = {
    { 1, 0x040D }, { 3, 0x0402 },
};

}  // namespace

extern const PreparedDefinition kDef_msh_AirQMon{
    .zigbee_models=kModels_msh_AirQMon, .zigbee_models_count=sizeof(kModels_msh_AirQMon)/sizeof(kModels_msh_AirQMon[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="msh.AirQMon", .vendor="Msh",
    .meta=nullptr, .exposes=kExp_msh_AirQMon, .exposes_count=sizeof(kExp_msh_AirQMon)/sizeof(kExp_msh_AirQMon[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_msh_AirQMon, .from_zigbee_count=sizeof(kFz_msh_AirQMon)/sizeof(kFz_msh_AirQMon[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_msh_AirQMon,
    .bindings_count           = sizeof(kBind_msh_AirQMon)/sizeof(kBind_msh_AirQMon[0]),
    .endpoint_map             = kEndpoints_msh_AirQMon,
    .endpoint_map_count       = sizeof(kEndpoints_msh_AirQMon)/sizeof(kEndpoints_msh_AirQMon[0]),
};

}  // namespace zhc::devices::msh
