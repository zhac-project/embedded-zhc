// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tapestry THPZ1 — occupancy + temperature + humidity sensor.
//
// z2m-source: tapestry.ts #THPZ1 —
//   fromZigbee: [fz.temperature, fz.humidity, fz.occupancy]
//   exposes:    [e.occupancy(), e.temperature(), e.humidity()]
//
// The auto-port mis-classified this as an IAS-zone device: it wired the
// generic kFzIasZone (cluster 0x0500) and emitted a phantom
// alarm/tamper/battery_low bundle bound to 0x0500. That dropped ALL three
// real channels (temperature 0x0402, humidity 0x0405, occupancy 0x0406),
// reducing a tri-channel sensor to a dead IAS stub. Re-ported to the three
// generic measurement decoders with matching exposes and binds.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::tapestry {
namespace {
const FzConverter* const kFz_THPZ1[] = {
    &::zhc::generic::kFzTemperature,   // msTemperatureMeasurement 0x0402 -> temperature
    &::zhc::generic::kFzHumidity,      // msRelativeHumidity       0x0405 -> humidity
    &::zhc::generic::kFzOccupancy,     // msOccupancySensing       0x0406 -> occupancy
};

constexpr const char* kModels_THPZ1[] = { "Presence Z1" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C",    nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0405},   // msRelativeHumidity
    {1, 0x0406},   // msOccupancySensing
};

extern const PreparedDefinition kDef_THPZ1{
    .zigbee_models=kModels_THPZ1, .zigbee_models_count=sizeof(kModels_THPZ1)/sizeof(kModels_THPZ1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="THPZ1", .vendor="Tapestry",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_THPZ1, .from_zigbee_count=sizeof(kFz_THPZ1)/sizeof(kFz_THPZ1[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::tapestry
