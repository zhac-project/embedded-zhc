// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ikea E2134 — VALLHORN wireless motion sensor.
//
// z2m-source: ikea.ts #E2134 — m.occupancy() + m.illuminance() +
// m.identify() + m.battery({voltage:true}).
//
// The auto-generated def wired only kFzBattery, leaving occupancy and
// illuminance — the device's primary sensor outputs — dead. VALLHORN
// reports real msOccupancySensing (0x0406) + msIlluminanceMeasurement
// (0x0400) attribute reports (unlike the older TRADFRI E1745 which
// fakes occupancy via a genOnOff commandOnWithTimedOff), so the generic
// kFzOccupancy + kFzIlluminance decoders apply directly.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ikea {
namespace {
const FzConverter* const kFz_E2134[] = {
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_E2134[] = { "VALLHORN Wireless Motion Sensor" };

constexpr Expose kExposes_E2134[] = {
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_E2134[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0406},   // msOccupancySensing
    {1, 0x0400},   // msIlluminanceMeasurement
};

}  // namespace

extern const PreparedDefinition kDef_E2134{
    .zigbee_models=kModels_E2134, .zigbee_models_count=sizeof(kModels_E2134)/sizeof(kModels_E2134[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E2134", .vendor="Ikea",
    .meta=nullptr, .exposes=kExposes_E2134, .exposes_count=sizeof(kExposes_E2134)/sizeof(kExposes_E2134[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E2134, .from_zigbee_count=sizeof(kFz_E2134)/sizeof(kFz_E2134[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_E2134, .bindings_count=sizeof(kBindings_E2134)/sizeof(kBindings_E2134[0]),
};

}  // namespace zhc::devices::ikea
