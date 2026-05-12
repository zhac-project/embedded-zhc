// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated evanell registry.
#include "zhc/devices/evanell_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::evanell {

extern const PreparedDefinition kDef_EZ200;

const PreparedDefinition* const kEvanellRegistry[] = {
    &kDef_EZ200,
};
const std::size_t kEvanellRegistryCount = sizeof(kEvanellRegistry) / sizeof(kEvanellRegistry[0]);

}  // namespace zhc::devices::evanell
