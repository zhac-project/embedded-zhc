// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: LinknLink eMotion Air battery-powered mmWave presence multi-sensor
// (z2m v26.92.0 parity, added v26.85.0).
// z2m-source: linknlink.ts #eMotion Air
//
// Fingerprint is modelID + manufacturerName. Upstream is a straight stack of
// standard measurement clusters: m.temperature/humidity/illuminance/occupancy
// /battery. The two command extends (m.commandsOnOff / m.commandsLevelCtrl)
// let the sensor act as a controller; those emit `action` events and are not
// modelled here -- the sensing half, which is what the device is sold as, is
// complete.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::linknlink {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels[] = { "eMotion Air" };
constexpr const char* kManus[]  = { "LinknLink" };

constexpr Expose kExposes[] = {
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, "Presence detected",          nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\u00b0C",   "Measured temperature",       nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",     "Measured relative humidity", nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx",    "Measured illuminance",       nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",     "Battery percentage",         nullptr, 0},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0001},
    {1, 0x0400},
    {1, 0x0402},
    {1, 0x0405},
    {1, 0x0406},
};
}  // namespace

extern const PreparedDefinition kDef_eMotion_Air{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="eMotion Air", .vendor="LinknLink",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
};

}  // namespace zhc::devices::linknlink
