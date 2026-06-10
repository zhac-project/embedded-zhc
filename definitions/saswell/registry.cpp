// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated saswell registry.
#include "zhc/devices/saswell_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::saswell {

extern const PreparedDefinition kDef_SEA801;

const PreparedDefinition* const kSaswellRegistry[] = {
    &kDef_SEA801,
};
const std::size_t kSaswellRegistryCount = sizeof(kSaswellRegistry) / sizeof(kSaswellRegistry[0]);

}  // namespace zhc::devices::saswell
