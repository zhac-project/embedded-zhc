// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Terncy TERNCY-DC01 — uses shared terncy converters.
// Temperature & contact sensor — temperature reported in 0.1 °C
// (m.temperature scale 10), contact via genBinaryInput.presentValue
// (m.binary), battery on standard genPowerCfg. The z2m
// `dontDividePercentage` flag means the device already reports
// battery as 0..100 (not half-percent) — kFzBattery is a raw
// pass-through, so no host-side adjustment is needed.
// z2m-source: terncy.ts #TERNCY-DC01.
#include "definitions/_generic/_shared.hpp"
#include "definitions/terncy/_shared.hpp"

namespace zhc::devices::terncy {
namespace {

const FzConverter* const kFz_TERNCY_DC01[] = {
    &::zhc::terncy::kFzTerncyTempScale10,
    &::zhc::terncy::kFzTerncyContact,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_TERNCY_DC01[] = { "TERNCY-DC01" };

constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"contact",     ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x000F},   // genBinaryInput (contact)
};

}  // namespace

extern const PreparedDefinition kDef_TERNCY_DC01{
    .zigbee_models=kModels_TERNCY_DC01,
    .zigbee_models_count=sizeof(kModels_TERNCY_DC01)/sizeof(kModels_TERNCY_DC01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TERNCY-DC01", .vendor="Terncy",
    .meta=nullptr,
    .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TERNCY_DC01, .from_zigbee_count=sizeof(kFz_TERNCY_DC01)/sizeof(kFz_TERNCY_DC01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::terncy
