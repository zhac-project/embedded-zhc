// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Perenio PECWS01 — hand-rewritten (vendor parity sweep).
// Door sensor.
// z2m-source: perenio.ts #PECWS01.
//
// Wire-level: ssIasZone → "contact" + "tamper" + "battery_low";
// genPowerCfg → "battery" + "voltage" (the latter exposed as
// `battery_voltage` to match z2m's e.battery_voltage()).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::perenio {
namespace {
const FzConverter* const kFz_PECWS01[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,
};

constexpr const char* kModels_PECWS01[] = { "ZHA-DoorLockSensor" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"battery",         ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"battery_voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"contact",         ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",          ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low",     ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone
};

extern const PreparedDefinition kDef_PECWS01{
    .zigbee_models=kModels_PECWS01, .zigbee_models_count=sizeof(kModels_PECWS01)/sizeof(kModels_PECWS01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PECWS01", .vendor="Perenio",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_PECWS01, .from_zigbee_count=sizeof(kFz_PECWS01)/sizeof(kFz_PECWS01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::perenio
