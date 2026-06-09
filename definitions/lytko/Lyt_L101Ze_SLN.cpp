// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lytko L101Ze-SLN — single channel thermostat without display.
//
// Hand-rewritten (lytko parity sweep): the legacy generator could not
// read the dynamic exposes produced by z2m's `modernExtend`
// `thermostat({...})` builder and lowered this device to an incorrect
// battery + onOff stub (state/battery/voltage on genOnOff 0x0006). The
// L101Ze family is a heating thermostat, not a switch. Mirrors the
// already-corrected sibling `L101Ze-SBN`: the core control surface
// (local_temperature / current_heating_setpoint / system_mode) decodes
// via the canonical generic kFzThermostat, and writes route through
// generic kTzThermostat plus the shared Lytko manuSpec encoders.
//
// Single channel => thermostat lives on endpoint 3 (z2m deviceEndpoints
// {1:1, 3:3}, thermostat endpointNames ["3"]).
//
// Config-only exposes that z2m surfaces via converters with no generic
// decoder equivalent (remote_sensing) are deliberately omitted — INFRA,
// deferred. sensor_type / occupied_setback are wired write-only through
// the shared Lytko TZ encoders.
//
// z2m-source: zigbee-herdsman-converters/src/devices/lytko.ts #L101Ze-SLN.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lytko/_shared.hpp"

namespace zhc::devices::lytko {
namespace {

constexpr const char* kModels_L101Ze_SLN[] = { "L101Ze-SLN" };

const FzConverter* const kFz_L101Ze_SLN[] = {
    &::zhc::generic::kFzThermostat,
};
const TzConverter* const kTz_L101Ze_SLN[] = {
    &::zhc::generic::kTzThermostat,
    &kTzLytkoSensorType,
    &kTzLytkoOccupiedSetback,
};

constexpr Expose kExp_L101Ze_SLN[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_mode", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"sensor_type", ExposeType::Enum, Access::StateSet, nullptr, "External sensor resistance kOhm", nullptr, 0},
    {"occupied_setback", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_L101Ze_SLN[] = {
    {3, 0x0201},  // hvacThermostat on endpoint 3
};
// Single channel: no endpoint_map ⇒ core keys stay un-suffixed, matching
// the sibling single-channel L101Z-SLN / L101Z-SBN defs and z2m's
// single-thermostat output. (The thermostat lives on ep 3 only, so there
// is no second channel to disambiguate.)

}  // namespace

extern const PreparedDefinition kDef_L101Ze_SLN{
    .zigbee_models=kModels_L101Ze_SLN, .zigbee_models_count=sizeof(kModels_L101Ze_SLN)/sizeof(kModels_L101Ze_SLN[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="L101Ze-SLN", .vendor="Lytko",
    .meta=nullptr, .exposes=kExp_L101Ze_SLN, .exposes_count=sizeof(kExp_L101Ze_SLN)/sizeof(kExp_L101Ze_SLN[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_L101Ze_SLN, .from_zigbee_count=sizeof(kFz_L101Ze_SLN)/sizeof(kFz_L101Ze_SLN[0]),
    .to_zigbee=kTz_L101Ze_SLN, .to_zigbee_count=sizeof(kTz_L101Ze_SLN)/sizeof(kTz_L101Ze_SLN[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_L101Ze_SLN, .bindings_count=sizeof(kBind_L101Ze_SLN)/sizeof(kBind_L101Ze_SLN[0]),
};

}  // namespace zhc::devices::lytko
