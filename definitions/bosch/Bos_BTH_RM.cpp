// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Bosch BTH-RM — Room thermostat II (RBSH-RTH0-BAT-ZB-EU).
// Was mis-classed as a Light (state/brightness/color_temp) and decoded
// no thermostat fields. Re-targeted to the bosch/_shared TRV bundle:
// generic hvacThermostat fz/tz (local_temperature / current_heating_
// setpoint / system_mode / running_state) + battery, plus the Bosch
// manu-specific TZ writes (operating_mode / boost_heating / window_open /
// remote_temperature / child_lock / display settings). Flat surface, no
// ExposeType::Climate (project rule).
// Gaps vs z2m: valve_adapt / error_state diagnostics are manuSpec reads
// not yet decoded — see docs/BOSCH_PARITY.md.
// z2m-source: bosch.ts #BTH-RM.
#include "definitions/_generic/_shared.hpp"
#include "definitions/bosch/_shared.hpp"

namespace zhc::devices::bosch {
namespace {


constexpr const char* kModels_BTH_RM[] = { "RBSH-RTH0-BAT-ZB-EU" };

}  // namespace

extern const PreparedDefinition kDef_BTH_RM{
    .zigbee_models=kModels_BTH_RM, .zigbee_models_count=sizeof(kModels_BTH_RM)/sizeof(kModels_BTH_RM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="BTH-RM", .vendor="Bosch",
    .meta=nullptr, .exposes=kExposesBoschTrv, .exposes_count=kExposesBoschTrvCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzBoschTrv, .from_zigbee_count=kFzBoschTrvCount,
    .to_zigbee=kTzBoschTrv, .to_zigbee_count=kTzBoschTrvCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsBoschTrv,
    .bindings_count           = kBindingsBoschTrvCount,
};

}  // namespace zhc::devices::bosch
