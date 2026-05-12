// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated wisdom registry.
#include "zhc/devices/wisdom_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::wisdom {

extern const PreparedDefinition kDef_DMZ250;

const PreparedDefinition* const kWisdomRegistry[] = {
    &kDef_DMZ250,
};
const std::size_t kWisdomRegistryCount = sizeof(kWisdomRegistry) / sizeof(kWisdomRegistry[0]);

}  // namespace zhc::devices::wisdom
