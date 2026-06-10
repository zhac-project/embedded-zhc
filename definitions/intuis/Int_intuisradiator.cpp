// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Intuis intuisradiator — graduated from generated/ 2026-06-10.
// Radiator with nativ and intuis 3.0 (Muller Intuitiv electric heater).
//
// z2m-source: intuis.ts #intuisradiator (fz.thermostat +
//             fz.hvac_user_interface + m.occupancy + m.electricityMeter).
//
// Parity fix (real gaps). The auto-generated wiring used the generic
// kFzMetering + kFzThermostat only, leaving most of the device dead:
//   * kFzThermostat emits attr 0x0012 as "current_heating_setpoint",
//     but the climate expose declares "occupied_heating_setpoint" →
//     dead key. It also never decodes 0x0014 (unoccupied_heating_
//     setpoint) or 0x0029 (running_state).
//   * No occupancy decoder was wired (msOccupancySensing 0x0406) →
//     the m.occupancy expose was dead.
//   * No hvacUserInterfaceCfg decoder (0x0204) → keypad_lockout and
//     temperature_display_mode were dead.
// Now routed through kFzIntuisThermostat + kFzIntuisHvacUi (see
// _shared) plus the generic kFzOccupancy. Exposes corrected:
// occupied/unoccupied setpoints, running_state, occupancy,
// keypad_lockout, temperature_display_mode; the phantom "power"
// expose is dropped (z2m sets electricityMeter power:false, so the
// device reports only energy via seMetering 0x0702 — the metering
// "0x0B04 half" SUSPECT is a false flag: z2m uses cluster:"metering",
// not "both").
//
// Endpoint note: z2m declares deviceEndpoints {1:1, 5:5} with
// meta.multiEndpoint, but every extend targets EP1 only
// (endpointNames:["1"] / .withEndpoint("1")), so EP5 carries no
// decoded key. The endpoint_map {"1",1} suffixes the EP1 climate /
// occupancy / energy keys to "_1" matching z2m's
// postfixWithEndpointName. keypad_lockout / temperature_display_mode
// arrive on EP1 too and are suffixed "_1" by the dispatch layer,
// whereas z2m leaves them bare (they have no .withEndpoint). This is
// a known limitation of the all-or-nothing per-endpoint suffix model
// (a single in-map endpoint suffixes every non-global key) — tracked
// as INFRA, not worth a per-key exemption for two diagnostic fields.
#include "definitions/_generic/_shared.hpp"
#include "definitions/intuis/_shared.hpp"

namespace zhc::devices::intuis {
namespace {
const FzConverter* const kFz_intuisradiator[] = {
    &::zhc::generic::kFzMetering,
    &kFzIntuisThermostat,
    &kFzIntuisHvacUi,
    &::zhc::generic::kFzOccupancy,
};
const TzConverter* const kTz_intuisradiator[] = {
    &::zhc::generic::kTzThermostat,
};
constexpr const char* kModels_intuisradiator[] = { "intuisradiator                 " };

constexpr ::zhc::EndpointLabel kEndpoints_intuisradiator[] = { {"1", 1} };

}  // namespace


// Exposes match z2m's intuisradiator surface. Keys stay bare here;
// the dispatch layer suffixes the runtime keys to "_1" per the
// endpoint_map (z2m postfixWithEndpointName). z2m sets metering
// power:false (no instantaneous demand) → only "energy" is exposed.
constexpr Expose kAutoExposes[] = {
    {"energy",                      ExposeType::Numeric, Access::State,    "kWh", nullptr, nullptr, 0},
    {"local_temperature",           ExposeType::Numeric, Access::State,    "C",   nullptr, nullptr, 0},
    {"occupied_heating_setpoint",   ExposeType::Numeric, Access::StateSet, "C",   nullptr, nullptr, 0},
    {"unoccupied_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C",   nullptr, nullptr, 0},
    {"running_state",               ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
    {"system_mode",                 ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"occupancy",                   ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"keypad_lockout",              ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"temperature_display_mode",    ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0702},  // seMetering on EP1
    {1, 0x0201},  // hvacThermostat on EP1
    {1, 0x0204},  // hvacUserInterfaceCfg on EP1
    {1, 0x0406},  // msOccupancySensing on EP1 (m.occupancy endpointNames:["1"])
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_intuisradiator{
    .zigbee_models=kModels_intuisradiator, .zigbee_models_count=sizeof(kModels_intuisradiator)/sizeof(kModels_intuisradiator[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="intuisradiator", .vendor="Intuis",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_intuisradiator, .from_zigbee_count=sizeof(kFz_intuisradiator)/sizeof(kFz_intuisradiator[0]),
    .to_zigbee=kTz_intuisradiator, .to_zigbee_count=sizeof(kTz_intuisradiator)/sizeof(kTz_intuisradiator[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_intuisradiator,
    .endpoint_map_count = sizeof(kEndpoints_intuisradiator)/sizeof(kEndpoints_intuisradiator[0]),
};

}  // namespace zhc::devices::intuis
