// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lytko L101Ze-DBN — dual channel thermostat with big display.
//
// Hand-rewritten (lytko parity sweep): the legacy generator could not
// read z2m's `modernExtend` `thermostat({...})` dynamic exposes and
// lowered this device to an incorrect battery + onOff stub. The L101Ze
// family is a heating thermostat. Mirrors the corrected sibling
// `L101Ze-SBN`: core surface via generic kFzThermostat, writes via
// generic kTzThermostat + shared Lytko encoders.
//
// DUAL channel: z2m deviceEndpoints {1:1, 3:3, 4:4} + two thermostat
// builders ⇒ two-entry endpoint_map drives `<key>_3` / `<key>_4` key
// suffixing for the per-channel thermostat surface. The big-display UI
// cfg (keypad lock + active/standby brightness + target-first) is a
// single device-level cluster on endpoint 1, so those write-only keys
// stay un-suffixed to match their shared Lytko TZ encoder `.key`.
//
// z2m-source: zigbee-herdsman-converters/src/devices/lytko.ts #L101Ze-DBN.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lytko/_shared.hpp"

namespace zhc::devices::lytko {
namespace {

constexpr const char* kModels_L101Ze_DBN[] = { "L101Ze-DBN" };

const FzConverter* const kFz_L101Ze_DBN[] = {
    &::zhc::generic::kFzThermostat,
};
const TzConverter* const kTz_L101Ze_DBN[] = {
    &::zhc::generic::kTzThermostat,
    &kTzLytkoSensorType,
    &kTzLytkoTargetTempFirst,
    &kTzLytkoOccupiedSetback,
    &kTzLytkoKeypadLockout,
    &kTzLytkoBrightnessActive,
    &kTzLytkoBrightnessStandby,
};

constexpr Expose kExp_L101Ze_DBN[] = {
    // Per-channel thermostat surface (suffixed _3 / _4 by dispatch).
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
    // Device-level config (endpoint 1; write-only manuSpec encoders).
    {"sensor_type", ExposeType::Enum, Access::StateSet, nullptr, "External sensor resistance kOhm", nullptr, 0},
    {"target_temp_first", ExposeType::Binary, Access::StateSet, nullptr, "Display target/current first", nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, "Physical input lock", nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, "%", "Display brightness (active)", nullptr, 0},
    {"brightness_standby", ExposeType::Numeric, Access::StateSet, "%", "Display brightness (standby)", nullptr, 0},
    {"occupied_setback", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_L101Ze_DBN[] = {
    {3, 0x0201},  // hvacThermostat on endpoint 3
    {4, 0x0201},  // hvacThermostat on endpoint 4
    {1, 0x0204},  // hvacUserInterfaceCfg on endpoint 1 (child_lock / brightness)
};
constexpr ::zhc::EndpointLabel kEndpoints_L101Ze_DBN[] = { {"3", 3}, {"4", 4} };

}  // namespace

extern const PreparedDefinition kDef_L101Ze_DBN{
    .zigbee_models=kModels_L101Ze_DBN, .zigbee_models_count=sizeof(kModels_L101Ze_DBN)/sizeof(kModels_L101Ze_DBN[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="L101Ze-DBN", .vendor="Lytko",
    .meta=nullptr, .exposes=kExp_L101Ze_DBN, .exposes_count=sizeof(kExp_L101Ze_DBN)/sizeof(kExp_L101Ze_DBN[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_L101Ze_DBN, .from_zigbee_count=sizeof(kFz_L101Ze_DBN)/sizeof(kFz_L101Ze_DBN[0]),
    .to_zigbee=kTz_L101Ze_DBN, .to_zigbee_count=sizeof(kTz_L101Ze_DBN)/sizeof(kTz_L101Ze_DBN[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_L101Ze_DBN, .bindings_count=sizeof(kBind_L101Ze_DBN)/sizeof(kBind_L101Ze_DBN[0]),
    .endpoint_map       = kEndpoints_L101Ze_DBN,
    .endpoint_map_count = sizeof(kEndpoints_L101Ze_DBN)/sizeof(kEndpoints_L101Ze_DBN[0]),
};

}  // namespace zhc::devices::lytko
