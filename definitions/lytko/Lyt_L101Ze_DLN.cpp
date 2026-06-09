// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lytko L101Ze-DLN — dual channel thermostat without display.
//
// Hand-rewritten (lytko parity sweep): the legacy generator could not
// read z2m's `modernExtend` `thermostat({...})` dynamic exposes and
// lowered this device to an incorrect battery + onOff stub. The L101Ze
// family is a heating thermostat. Mirrors the corrected sibling
// `L101Ze-SBN`: the core surface decodes via generic kFzThermostat,
// writes route through generic kTzThermostat + shared Lytko encoders.
//
// DUAL channel: z2m deviceEndpoints {1:1, 3:3, 4:4} with two
// `thermostat({endpointNames:["3"]/["4"]})` builders. With a two-entry
// `endpoint_map`, the dispatch layer rewrites every per-endpoint key to
// `<key>_3` / `<key>_4` (src_endpoint-driven), so the exposes below are
// declared with the matching suffixes — otherwise they would be dead.
//
// Omitted as INFRA (no generic decoder equivalent): remote_sensing
// config enum (per channel). sensor_type / occupied_setback wired
// write-only via the shared Lytko TZ encoders.
//
// z2m-source: zigbee-herdsman-converters/src/devices/lytko.ts #L101Ze-DLN.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lytko/_shared.hpp"

namespace zhc::devices::lytko {
namespace {

constexpr const char* kModels_L101Ze_DLN[] = { "L101Ze-DLN" };

const FzConverter* const kFz_L101Ze_DLN[] = {
    &::zhc::generic::kFzThermostat,
};
const TzConverter* const kTz_L101Ze_DLN[] = {
    &::zhc::generic::kTzThermostat,
    &kTzLytkoSensorType,
    &kTzLytkoOccupiedSetback,
};

// Per-endpoint suffixed surface (channels 3 and 4). The runtime rewrites
// generic kFzThermostat output to `<key>_<endpoint>` via endpoint_map.
constexpr Expose kExp_L101Ze_DLN[] = {
    {"local_temperature_3", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint_3", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode_3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_mode_3", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"local_temperature_calibration_3", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"local_temperature_4", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint_4", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode_4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_mode_4", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"local_temperature_calibration_4", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"sensor_type", ExposeType::Enum, Access::StateSet, nullptr, "External sensor resistance kOhm", nullptr, 0},
    {"occupied_setback", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_L101Ze_DLN[] = {
    {3, 0x0201},  // hvacThermostat on endpoint 3
    {4, 0x0201},  // hvacThermostat on endpoint 4
};
constexpr ::zhc::EndpointLabel kEndpoints_L101Ze_DLN[] = { {"3", 3}, {"4", 4} };

}  // namespace

extern const PreparedDefinition kDef_L101Ze_DLN{
    .zigbee_models=kModels_L101Ze_DLN, .zigbee_models_count=sizeof(kModels_L101Ze_DLN)/sizeof(kModels_L101Ze_DLN[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="L101Ze-DLN", .vendor="Lytko",
    .meta=nullptr, .exposes=kExp_L101Ze_DLN, .exposes_count=sizeof(kExp_L101Ze_DLN)/sizeof(kExp_L101Ze_DLN[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_L101Ze_DLN, .from_zigbee_count=sizeof(kFz_L101Ze_DLN)/sizeof(kFz_L101Ze_DLN[0]),
    .to_zigbee=kTz_L101Ze_DLN, .to_zigbee_count=sizeof(kTz_L101Ze_DLN)/sizeof(kTz_L101Ze_DLN[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_L101Ze_DLN, .bindings_count=sizeof(kBind_L101Ze_DLN)/sizeof(kBind_L101Ze_DLN[0]),
    .endpoint_map       = kEndpoints_L101Ze_DLN,
    .endpoint_map_count = sizeof(kEndpoints_L101Ze_DLN)/sizeof(kEndpoints_L101Ze_DLN[0]),
};

}  // namespace zhc::devices::lytko
