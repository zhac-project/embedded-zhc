// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lytko L101Ze-SLM — single channel thermostat without display
// (with power metering).
//
// Hand-rewritten (lytko parity sweep): the legacy generator could not
// read z2m's `modernExtend` `thermostat({...})` dynamic exposes and
// lowered this device to an incorrect battery + onOff stub. The L101Ze
// family is a heating thermostat. Mirrors the corrected sibling
// `L101Ze-SBN`: the core surface decodes via generic kFzThermostat,
// writes route through generic kTzThermostat + shared Lytko encoders.
//
// Single channel => thermostat on endpoint 3 (z2m deviceEndpoints
// {1:1, 3:3}, thermostat endpointNames ["3"]).
//
// Omitted as INFRA (no generic decoder equivalent): z2m's
// electricityMeter() power read and remote_sensing config enum.
// sensor_type / occupied_setback wired write-only via shared Lytko TZ.
//
// z2m-source: zigbee-herdsman-converters/src/devices/lytko.ts #L101Ze-SLM.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lytko/_shared.hpp"

namespace zhc::devices::lytko {
namespace {

constexpr const char* kModels_L101Ze_SLM[] = { "L101Ze-SLM" };

const FzConverter* const kFz_L101Ze_SLM[] = {
    &::zhc::generic::kFzThermostat,
};
const TzConverter* const kTz_L101Ze_SLM[] = {
    &::zhc::generic::kTzThermostat,
    &kTzLytkoSensorType,
    &kTzLytkoOccupiedSetback,
};

constexpr Expose kExp_L101Ze_SLM[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_mode", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"sensor_type", ExposeType::Enum, Access::StateSet, nullptr, "External sensor resistance kOhm", nullptr, 0},
    {"occupied_setback", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_L101Ze_SLM[] = {
    {3, 0x0201},  // hvacThermostat on endpoint 3
};
// Single channel: no endpoint_map ⇒ core keys stay un-suffixed (see
// the sibling single-channel L101Z-* defs).

}  // namespace

extern const PreparedDefinition kDef_L101Ze_SLM{
    .zigbee_models=kModels_L101Ze_SLM, .zigbee_models_count=sizeof(kModels_L101Ze_SLM)/sizeof(kModels_L101Ze_SLM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="L101Ze-SLM", .vendor="Lytko",
    .meta=nullptr, .exposes=kExp_L101Ze_SLM, .exposes_count=sizeof(kExp_L101Ze_SLM)/sizeof(kExp_L101Ze_SLM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_L101Ze_SLM, .from_zigbee_count=sizeof(kFz_L101Ze_SLM)/sizeof(kFz_L101Ze_SLM[0]),
    .to_zigbee=kTz_L101Ze_SLM, .to_zigbee_count=sizeof(kTz_L101Ze_SLM)/sizeof(kTz_L101Ze_SLM[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_L101Ze_SLM, .bindings_count=sizeof(kBind_L101Ze_SLM)/sizeof(kBind_L101Ze_SLM[0]),
};

}  // namespace zhc::devices::lytko
