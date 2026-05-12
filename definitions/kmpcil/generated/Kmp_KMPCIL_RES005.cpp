// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Kmpcil KMPCIL_RES005 — environment sensor (multi-cluster).
//
// z2m-source: kmpcil.ts #KMPCIL_RES005 (zigbeeModel "RES005").
// All cluster traffic happens on endpoint 8. Wires:
//   - genPowerCfg                 → battery (% / mV)         [generic kFzBattery]
//   - msTemperatureMeasurement    → temperature              [generic kFzTemperature]
//   - msRelativeHumidity          → humidity                 [generic kFzHumidity]
//   - msPressureMeasurement       → pressure                 [generic kFzPressure]
//   - msIlluminanceMeasurement    → illuminance              [generic kFzIlluminance]
//                                    (z2m's `extend: [m.illuminance()]`)
//   - genBinaryInput  attr 0x55   → occupancy                [shared kFzKmpcilRES005Occupancy]
//   - genBinaryOutput attr 0x55   → state (on/off)           [shared kFzKmpcilRES005OnOff]
//   - genBinaryOutput attr 0x55 ← state writeAttributes      [shared kTzKmpcilRES005OnOff]
#include "definitions/_generic/_shared.hpp"
#include "definitions/kmpcil/_shared.hpp"

namespace zhc::devices::kmpcil {
namespace {

const FzConverter* const kFz_KMPCIL_RES005[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzPressure,
    &::zhc::generic::kFzIlluminance,
    &kFzKmpcilRES005Occupancy,
    &kFzKmpcilRES005OnOff,
};

const TzConverter* const kTz_KMPCIL_RES005[] = {
    &kTzKmpcilRES005OnOff,
};

constexpr const char* kModels_KMPCIL_RES005[] = { "RES005" };

constexpr Expose kExp_KMPCIL_RES005[] = {
    { "battery",     ExposeType::Numeric, ::zhc::Access::State,    "%",   nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "voltage",     ExposeType::Numeric, ::zhc::Access::State,    "mV",  nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "temperature", ExposeType::Numeric, ::zhc::Access::State,    "°C",  nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, ::zhc::Access::State,    "%",   nullptr, nullptr, 0 },
    { "pressure",    ExposeType::Numeric, ::zhc::Access::State,    "hPa", nullptr, nullptr, 0 },
    { "illuminance", ExposeType::Numeric, ::zhc::Access::State,    "lx",  nullptr, nullptr, 0 },
    { "occupancy",   ExposeType::Binary,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "state",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};

// Single-endpoint device on EP 8 (z2m's `device.getEndpoint(8)` for
// every cluster). Bind every cluster used at join time.
constexpr BindingSpec kBind_KMPCIL_RES005[] = {
    { 8, 0x0001 },   // genPowerCfg
    { 8, 0x0402 },   // msTemperatureMeasurement
    { 8, 0x0405 },   // msRelativeHumidity
    { 8, 0x0403 },   // msPressureMeasurement
    { 8, 0x0400 },   // msIlluminanceMeasurement
    { 8, 0x000F },   // genBinaryInput
    { 8, 0x0010 },   // genBinaryOutput
};

}  // namespace

extern const PreparedDefinition kDef_KMPCIL_RES005{
    .zigbee_models=kModels_KMPCIL_RES005,
    .zigbee_models_count=sizeof(kModels_KMPCIL_RES005)/sizeof(kModels_KMPCIL_RES005[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="KMPCIL_RES005", .vendor="KMPCIL",
    .meta=nullptr,
    .exposes=kExp_KMPCIL_RES005,
    .exposes_count=sizeof(kExp_KMPCIL_RES005)/sizeof(kExp_KMPCIL_RES005[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_KMPCIL_RES005,
    .from_zigbee_count=sizeof(kFz_KMPCIL_RES005)/sizeof(kFz_KMPCIL_RES005[0]),
    .to_zigbee=kTz_KMPCIL_RES005,
    .to_zigbee_count=sizeof(kTz_KMPCIL_RES005)/sizeof(kTz_KMPCIL_RES005[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_KMPCIL_RES005,
    .bindings_count=sizeof(kBind_KMPCIL_RES005)/sizeof(kBind_KMPCIL_RES005[0]),
};

}  // namespace zhc::devices::kmpcil
