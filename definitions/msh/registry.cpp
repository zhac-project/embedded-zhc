// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated msh registry.
#include "zhc/devices/msh_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::msh {

extern const PreparedDefinition kDef_msh_bme280psm;
extern const PreparedDefinition kDef_msh_ds18b20psm;
extern const PreparedDefinition kDef_msh_AirQMon;
extern const PreparedDefinition kDef_msh_pzem;

const PreparedDefinition* const kMshRegistry[] = {
    &kDef_msh_bme280psm,
    &kDef_msh_ds18b20psm,
    &kDef_msh_AirQMon,
    &kDef_msh_pzem,
};
const std::size_t kMshRegistryCount = sizeof(kMshRegistry) / sizeof(kMshRegistry[0]);

}  // namespace zhc::devices::msh
