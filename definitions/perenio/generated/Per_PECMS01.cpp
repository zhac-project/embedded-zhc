// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Perenio PECMS01 — hand-rewritten (vendor parity sweep).
// Motion sensor.
// z2m-source: perenio.ts #PECMS01.
//
// Matched on (modelID="ZHA-PirSensor", manufacturerName="LDS"). The
// modelID lands in `zigbee_models`; the manuf-name discriminator is
// kept in `manufacturer_names` for completeness.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::perenio {
namespace {
const FzConverter* const kFz_PECMS01[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_PECMS01[] = { "ZHA-PirSensor" };
constexpr const char* kManufNames_PECMS01[] = { "LDS" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone
};

extern const PreparedDefinition kDef_PECMS01{
    .zigbee_models=kModels_PECMS01, .zigbee_models_count=sizeof(kModels_PECMS01)/sizeof(kModels_PECMS01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManufNames_PECMS01,
    .manufacturer_names_count=sizeof(kManufNames_PECMS01)/sizeof(kManufNames_PECMS01[0]),
    .model="PECMS01", .vendor="Perenio",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_PECMS01, .from_zigbee_count=sizeof(kFz_PECMS01)/sizeof(kFz_PECMS01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::perenio
