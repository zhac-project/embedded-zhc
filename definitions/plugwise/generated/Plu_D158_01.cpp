// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Plugwise 158-01 — uses generic converters only.
// Lisa zone thermostat. z2m extends `m.thermostat` (occupiedHeatingSetpoint
// 0..30 step 0.5, systemMode {off, auto}) + `m.battery` — no plugwise
// manuSpec attributes. The generic kFz/kTzThermostat covers all of it.
// z2m-source: plugwise.ts #158-01.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::plugwise {
namespace {
const FzConverter* const kFz_D158_01[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzThermostat,
};
const TzConverter* const kTz_D158_01[] = {
    &::zhc::generic::kTzThermostat,
};

constexpr const char* kModels_D158_01[] = { "158-01" };

}  // namespace


// --- exposes ---
constexpr Expose kAutoExposes[] = {
    {"battery",                   ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"voltage",                   ExposeType::Numeric, Access::State,    "mV", nullptr, nullptr, 0},
    {"local_temperature",         ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"occupied_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C",  nullptr, nullptr, 0},
    {"system_mode",               ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0201},   // hvacThermostat
};
// --- end ---

extern const PreparedDefinition kDef_D158_01{
    .zigbee_models=kModels_D158_01, .zigbee_models_count=sizeof(kModels_D158_01)/sizeof(kModels_D158_01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="158-01", .vendor="Plugwise",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D158_01, .from_zigbee_count=sizeof(kFz_D158_01)/sizeof(kFz_D158_01[0]),
    .to_zigbee=kTz_D158_01, .to_zigbee_count=sizeof(kTz_D158_01)/sizeof(kTz_D158_01[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::plugwise
