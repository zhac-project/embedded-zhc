// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lytko L101Ze-DLM — dual channel thermostat without display
// (with power metering).
//
// Hand-rewritten (lytko parity sweep): the legacy generator could not
// read z2m's `modernExtend` `thermostat({...})` dynamic exposes and
// lowered this device to an incorrect battery + onOff stub. The L101Ze
// family is a heating thermostat. Mirrors the corrected sibling
// `L101Ze-SBN`: core surface via generic kFzThermostat, writes via
// generic kTzThermostat + shared Lytko encoders.
//
// DUAL channel: z2m deviceEndpoints {1:1, 3:3, 4:4} + two thermostat
// builders. Two-entry endpoint_map drives `<key>_3` / `<key>_4` key
// suffixing in dispatch, so the exposes carry the matching suffixes.
//
// Omitted as INFRA (no generic decoder equivalent): electricityMeter()
// power read + remote_sensing config enum. sensor_type / occupied_setback
// wired write-only via the shared Lytko TZ encoders.
//
// z2m-source: zigbee-herdsman-converters/src/devices/lytko.ts #L101Ze-DLM.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lytko/_shared.hpp"

namespace zhc::devices::lytko {
namespace {

constexpr const char* kModels_L101Ze_DLM[] = { "L101Ze-DLM" };

const FzConverter* const kFz_L101Ze_DLM[] = {
    &::zhc::generic::kFzThermostat,
};
const TzConverter* const kTz_L101Ze_DLM[] = {
    &::zhc::generic::kTzThermostat,
    &kTzLytkoSensorType,
    &kTzLytkoOccupiedSetback,
};

constexpr Expose kExp_L101Ze_DLM[] = {
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

constexpr BindingSpec kBind_L101Ze_DLM[] = {
    {3, 0x0201},  // hvacThermostat on endpoint 3
    {4, 0x0201},  // hvacThermostat on endpoint 4
};
constexpr ::zhc::EndpointLabel kEndpoints_L101Ze_DLM[] = { {"3", 3}, {"4", 4} };

}  // namespace

extern const PreparedDefinition kDef_L101Ze_DLM{
    .zigbee_models=kModels_L101Ze_DLM, .zigbee_models_count=sizeof(kModels_L101Ze_DLM)/sizeof(kModels_L101Ze_DLM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="L101Ze-DLM", .vendor="Lytko",
    .meta=nullptr, .exposes=kExp_L101Ze_DLM, .exposes_count=sizeof(kExp_L101Ze_DLM)/sizeof(kExp_L101Ze_DLM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_L101Ze_DLM, .from_zigbee_count=sizeof(kFz_L101Ze_DLM)/sizeof(kFz_L101Ze_DLM[0]),
    .to_zigbee=kTz_L101Ze_DLM, .to_zigbee_count=sizeof(kTz_L101Ze_DLM)/sizeof(kTz_L101Ze_DLM[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_L101Ze_DLM, .bindings_count=sizeof(kBind_L101Ze_DLM)/sizeof(kBind_L101Ze_DLM[0]),
    .endpoint_map       = kEndpoints_L101Ze_DLM,
    .endpoint_map_count = sizeof(kEndpoints_L101Ze_DLM)/sizeof(kEndpoints_L101Ze_DLM[0]),
};

}  // namespace zhc::devices::lytko
