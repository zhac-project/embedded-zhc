// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bosch BTH-RA — Radiator thermostat II.
// Wires the bosch/_shared TRV bundle (generic hvacThermostat fz/tz +
// battery) and the manu-specific TZ converters (operating_mode,
// boost_heating, window_open, remote_temperature, child_lock, display
// settings). Generic z2m thermostat fields (local_temperature,
// current_heating_setpoint, system_mode) ride on the standard
// hvacThermostat path.
//
// Gaps vs z2m: pi_heating_demand (boschThermostatExtend.customHeatingDemand,
// manuSpec heatingDemand 0x4020 ENUM8) is not yet decoded — needs a small
// fz_zcl helper. valve_adapt_status read + trigger_valve_adaptation are
// pure manuSpec reads/commands and only the write path is provided
// here. See docs/BOSCH_PARITY.md.
//
// z2m-source: bosch.ts #BTH-RA, lib/bosch.ts boschThermostatExtend.
#include "definitions/_generic/_shared.hpp"
#include "definitions/bosch/_shared.hpp"

namespace zhc::devices::bosch {
namespace {
constexpr const char* kModels_BTH_RA[] = {
    "RBSH-TRV0-ZB-EU", "RBSH-TRV1-ZB-EU",
};
}  // namespace

extern const PreparedDefinition kDef_BTH_RA{
    .zigbee_models           = kModels_BTH_RA,
    .zigbee_models_count     = sizeof(kModels_BTH_RA)/sizeof(kModels_BTH_RA[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "BTH-RA",
    .vendor                  = "Bosch",
    .meta                    = nullptr,
    .exposes                 = kExposesBoschTrv,
    .exposes_count           = kExposesBoschTrvCount,
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFzBoschTrv,
    .from_zigbee_count       = kFzBoschTrvCount,
    .to_zigbee               = kTzBoschTrv,
    .to_zigbee_count         = kTzBoschTrvCount,
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kBindingsBoschTrv,
    .bindings_count          = kBindingsBoschTrvCount,
};

}  // namespace zhc::devices::bosch
