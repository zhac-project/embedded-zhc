// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated ecozy registry.
#include "zhc/devices/ecozy_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::ecozy {

extern const PreparedDefinition kDef_D1TST_EU;

const PreparedDefinition* const kEcozyRegistry[] = {
    &kDef_D1TST_EU,
};
const std::size_t kEcozyRegistryCount = sizeof(kEcozyRegistry) / sizeof(kEcozyRegistry[0]);

}  // namespace zhc::devices::ecozy
