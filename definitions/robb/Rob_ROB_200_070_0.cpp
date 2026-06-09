// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Robb ROB_200-070-0 — graduated from generated/ to fix a real parity gap.
// Battery-powered PIR presence + temperature + humidity + light sensor.
// z2m-source: robb.ts #ROB_200-070-0 — m.deviceEndpoints({1..5}), m.battery(),
//   m.identify(), m.occupancy(), m.iasZoneAlarm({zoneType:"generic",
//   zoneAttributes:[alarm_1,alarm_2,tamper,battery_low]}),
//   m.temperature({endpointNames:["3"]}), m.humidity({endpointNames:["4"]}),
//   m.illuminance({endpointNames:["5"]}).
//
// PARITY FIX (missing core sensors): the auto-generated port lowered only
// kFzBattery + kFzIasZone, so this 4-measurement PIR/climate/lux sensor surfaced
// only its IAS alarm bits + battery and dropped occupancy, temperature, humidity
// AND illuminance — the entire point of the device. Add kFzOccupancy +
// kFzTemperature + kFzHumidity + kFzIlluminance (generic decoders match z2m's
// msOccupancySensing bit-0, /100 °C, /100 %RH, raw lux) plus the matching exposes
// and per-endpoint cluster binds. Mirrors the twin Sunricher HK-SENSOR-4IN1-A.
// occupancy + IAS report on ep1 (unmapped → bare keys); temperature/humidity/
// illuminance report on ep3/4/5 and the runtime suffixes them <key>_<n> via the
// endpoint_map, matching z2m's withEndpoint("3"/"4"/"5").
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::robb {
namespace {
const FzConverter* const kFz_ROB_200_070_0[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasZone,
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
};

constexpr const char* kModels_ROB_200_070_0[] = { "ROB_200-070-0" };

constexpr ::zhc::EndpointLabel kEndpoints_ROB_200_070_0[] = { {"3", 3}, {"4", 4}, {"5", 5} };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"alarm",       ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone
    {1, 0x0406},  // msOccupancySensing
    {3, 0x0402},  // msTemperatureMeasurement
    {4, 0x0405},  // msRelativeHumidity
    {5, 0x0400},  // msIlluminanceMeasurement
};

extern const PreparedDefinition kDef_ROB_200_070_0{
    .zigbee_models=kModels_ROB_200_070_0, .zigbee_models_count=sizeof(kModels_ROB_200_070_0)/sizeof(kModels_ROB_200_070_0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ROB_200-070-0", .vendor="Robb",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ROB_200_070_0, .from_zigbee_count=sizeof(kFz_ROB_200_070_0)/sizeof(kFz_ROB_200_070_0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_ROB_200_070_0,
    .endpoint_map_count = sizeof(kEndpoints_ROB_200_070_0)/sizeof(kEndpoints_ROB_200_070_0[0]),
};

}  // namespace zhc::devices::robb
