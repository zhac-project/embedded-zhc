// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Imhotepcreation BRI4P — underfloor-heating bridge, 16
// per-endpoint heat+cool thermostat channels (l1..l16).
// BRI4P Bridge for underfloor heating central and local thermostats.
//
// Parity fix 2026-06-10: the auto-port carried only local_temperature /
// current_heating_setpoint / system_mode and the generic kFzThermostat
// (0x0000/0x0012/0x001C). z2m's BRI4P exposes per endpoint a cooling
// setpoint plus min/max heat AND cool setpoint limits, all decoded by
// fz.thermostat (attrs 0x0011/0x0015/0x0016/0x0017/0x0018) — so those
// were entirely missing (no expose + no decoder). Added the cooling
// setpoint + four limit exposes, wired kFzImhotepThermostatExtras for the
// read path, and wired the generic min/max heat/cool setpoint-limit tz
// converters to mirror z2m's toZigbee. (Cooling-setpoint write mirrors
// the Owon precedent: no generic occupied_cooling_setpoint encoder, so
// kTzThermostat covers heating + system_mode only.)
// z2m-source: imhotepcreation.ts #BRI4P + fromZigbee.ts fz.thermostat.
#include "definitions/_generic/_shared.hpp"
#include "definitions/imhotepcreation/_shared.hpp"

namespace zhc::devices::imhotepcreation {
namespace {
const FzConverter* const kFz_BRI4P[] = {
    &::zhc::generic::kFzThermostat,
    &::zhc::imhotepcreation::kFzImhotepThermostatExtras,
};
const TzConverter* const kTz_BRI4P[] = {
    &::zhc::generic::kTzThermostat,
    &::zhc::generic::kTzMinHeatSetpointLimit,
    &::zhc::generic::kTzMaxHeatSetpointLimit,
    &::zhc::generic::kTzMinCoolSetpointLimit,
    &::zhc::generic::kTzMaxCoolSetpointLimit,
    &::zhc::generic::kTzSetpointRaiseLower,
};
constexpr const char* kModels_BRI4P[] = { "BRI4P" };

constexpr ::zhc::EndpointLabel kEndpoints_BRI4P[] = { {"l1", 1}, {"l2", 2}, {"l3", 3}, {"l4", 4}, {"l5", 5}, {"l6", 6}, {"l7", 7}, {"l8", 8}, {"l9", 9}, {"l10", 10}, {"l11", 11}, {"l12", 12}, {"l13", 13}, {"l14", 14}, {"l15", 15}, {"l16", 16} };

}  // namespace


// Exposes mirror z2m's per-endpoint climate: heat + cool setpoints plus
// min/max setpoint limits for both. Keys stay BARE — dispatch suffixes
// them to <key>_l<n> for the endpoints in kEndpoints_BRI4P.
constexpr Expose kAutoExposes[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"current_cooling_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"min_heat_setpoint_limit", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"max_heat_setpoint_limit", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"min_cool_setpoint_limit", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"max_cool_setpoint_limit", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0201},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_BRI4P{
    .zigbee_models=kModels_BRI4P, .zigbee_models_count=sizeof(kModels_BRI4P)/sizeof(kModels_BRI4P[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="BRI4P", .vendor="Imhotepcreation",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_BRI4P, .from_zigbee_count=sizeof(kFz_BRI4P)/sizeof(kFz_BRI4P[0]),
    .to_zigbee=kTz_BRI4P, .to_zigbee_count=sizeof(kTz_BRI4P)/sizeof(kTz_BRI4P[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_BRI4P,
    .endpoint_map_count = sizeof(kEndpoints_BRI4P)/sizeof(kEndpoints_BRI4P[0]),
};

}  // namespace zhc::devices::imhotepcreation
