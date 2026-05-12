// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated netica registry.
#include "zhc/devices/netica_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::netica {

extern const PreparedDefinition kDef_FRZ1;

const PreparedDefinition* const kNeticaRegistry[] = {
    &kDef_FRZ1,
};
const std::size_t kNeticaRegistryCount = sizeof(kNeticaRegistry) / sizeof(kNeticaRegistry[0]);

}  // namespace zhc::devices::netica
