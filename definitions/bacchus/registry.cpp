// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated bacchus registry.
#include "zhc/devices/bacchus_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::bacchus {

extern const PreparedDefinition kDef_Bacchus_Water_level_meter;
extern const PreparedDefinition kDef_Duck_Pool_Thermometer;
extern const PreparedDefinition kDef_Flower_Sensor_v2;
extern const PreparedDefinition kDef_Flower_Sensor_v4;
extern const PreparedDefinition kDef_Mercury_3ph_Counter;
extern const PreparedDefinition kDef_Mercury_Counter;
extern const PreparedDefinition kDef_Water_Station;
extern const PreparedDefinition kDef_Presence_Sensor_v2;
extern const PreparedDefinition kDef_Presence_Sensor_v2_6;

const PreparedDefinition* const kBacchusRegistry[] = {
    &kDef_Bacchus_Water_level_meter,
    &kDef_Duck_Pool_Thermometer,
    &kDef_Flower_Sensor_v2,
    &kDef_Flower_Sensor_v4,
    &kDef_Mercury_3ph_Counter,
    &kDef_Mercury_Counter,
    &kDef_Water_Station,
    &kDef_Presence_Sensor_v2,
    &kDef_Presence_Sensor_v2_6,
};
const std::size_t kBacchusRegistryCount = sizeof(kBacchusRegistry) / sizeof(kBacchusRegistry[0]);

}  // namespace zhc::devices::bacchus
