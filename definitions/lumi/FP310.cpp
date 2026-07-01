// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Aqara FP310 presence sensor (z2m v26.76.0 parity).
// z2m-source: lumi.ts #FP310
//
// z2m: m.temperature()/m.humidity()/m.illuminance() (standard ZCL clusters) +
// lumiBattery(voltageAttribute 0x0017) + fp1ePresence() (0xFCC0 mmWave) +
// presence_sensitivity / distance config. Ported here as the standard ZCL
// channels: temperature (0x0402 /100), humidity (0x0405 /100), illuminance
// (0x0400) and occupancy (msOccupancySensing 0x0406).
//
// DEFERRED (all Aqara 0xFCC0 manuSpecificLumi): mmWave presence readout
// (fp1ePresence), presence_sensitivity, approach/distance settings, and the
// lumiBattery voltage attribute (0x0017). NOTE: `occupancy` populates only if
// the device mirrors presence onto standard msOccupancySensing; its native
// presence rides the deferred 0xFCC0 cluster.
#include "definitions/lumi/_shared.hpp"
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lumi {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzOccupancy,
};
constexpr const char* kModels[] = { "lumi.sensor_occupy.acn1" };
}  // namespace

constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0405},  // msRelativeHumidity
    {1, 0x0400},  // msIlluminanceMeasurement
    {1, 0x0406},  // msOccupancySensing
};

extern const PreparedDefinition kDefFP310{
    .zigbee_models=kModels,.zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .model="FP310",.vendor="Xiaomi",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=nullptr,.to_zigbee_count=0,
    .configure=nullptr,.on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::lumi
