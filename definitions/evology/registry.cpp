// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated evology registry.
#include "zhc/devices/evology_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::evology {

extern const PreparedDefinition kDef_PSE03_V1_1_0;

const PreparedDefinition* const kEvologyRegistry[] = {
    &kDef_PSE03_V1_1_0,
};
const std::size_t kEvologyRegistryCount = sizeof(kEvologyRegistry) / sizeof(kEvologyRegistry[0]);

}  // namespace zhc::devices::evology
