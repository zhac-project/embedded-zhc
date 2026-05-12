// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Terncy TERNCY-PP01 — uses shared terncy converters.
// Awareness switch — emits action (single/double/triple/quadruple),
// occupancy via the manuSpecificClusterAduroSmart raw frame +
// motion-side payload, plus temperature (scale 10), illuminance, and
// battery on stock clusters. No outbound writes (toZigbee: []).
// z2m-source: terncy.ts #TERNCY-PP01.
#include "definitions/_generic/_shared.hpp"
#include "definitions/terncy/_shared.hpp"

namespace zhc::devices::terncy {
namespace {

const FzConverter* const kFz_TERNCY_PP01[] = {
    &::zhc::generic::kFzOccupancy,                 // msOccupancySensing 0x0406
    &::zhc::terncy::kFzTerncyRawActionMotion,      // raw button + motion side
    &::zhc::terncy::kFzTerncyTempScale10,          // 0.1 °C scale
    &::zhc::generic::kFzIlluminance,               // 0x0400 raw lux
    &::zhc::generic::kFzBattery,                   // 0x0001 voltage + battery%
};

constexpr const char* kModels_TERNCY_PP01[] = { "TERNCY-PP01" };

constexpr Expose kAutoExposes[] = {
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"action",      ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_side", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C",    nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx",    nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0400},   // msIlluminanceMeasurement
    {1, 0x0406},   // msOccupancySensing
    {1, 0xFCCC},   // manuSpecificClusterAduroSmart
};

}  // namespace

extern const PreparedDefinition kDef_TERNCY_PP01{
    .zigbee_models=kModels_TERNCY_PP01,
    .zigbee_models_count=sizeof(kModels_TERNCY_PP01)/sizeof(kModels_TERNCY_PP01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TERNCY-PP01", .vendor="Terncy",
    .meta=nullptr,
    .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TERNCY_PP01, .from_zigbee_count=sizeof(kFz_TERNCY_PP01)/sizeof(kFz_TERNCY_PP01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::terncy
