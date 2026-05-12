// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated simpla_home registry.
#include "zhc/devices/simpla_home_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::simpla_home {

extern const PreparedDefinition kDef_Soil_Pro;

const PreparedDefinition* const kSimplaHomeRegistry[] = {
    &kDef_Soil_Pro,
};
const std::size_t kSimplaHomeRegistryCount = sizeof(kSimplaHomeRegistry) / sizeof(kSimplaHomeRegistry[0]);

}  // namespace zhc::devices::simpla_home
