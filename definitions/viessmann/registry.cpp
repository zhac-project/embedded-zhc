// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated viessmann registry.
#include "zhc/devices/viessmann_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::viessmann {

extern const PreparedDefinition kDef_ZK03840;
extern const PreparedDefinition kDef_D7377019;
extern const PreparedDefinition kDef_D7963223;
extern const PreparedDefinition kDef_ZK03839;

const PreparedDefinition* const kViessmannRegistry[] = {
    &kDef_ZK03840,
    &kDef_D7377019,
    &kDef_D7963223,
    &kDef_ZK03839,
};
const std::size_t kViessmannRegistryCount = sizeof(kViessmannRegistry) / sizeof(kViessmannRegistry[0]);

}  // namespace zhc::devices::viessmann
