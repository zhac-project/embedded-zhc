// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated hilux registry.
#include "zhc/devices/hilux_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::hilux {

extern const PreparedDefinition kDef_D160_ZG;
extern const PreparedDefinition kDef_DZ8;

const PreparedDefinition* const kHiluxRegistry[] = {
    &kDef_D160_ZG,
    &kDef_DZ8,
};
const std::size_t kHiluxRegistryCount = sizeof(kHiluxRegistry) / sizeof(kHiluxRegistry[0]);

}  // namespace zhc::devices::hilux
