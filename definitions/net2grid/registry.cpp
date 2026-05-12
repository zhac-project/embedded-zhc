// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated net2grid registry.
#include "zhc/devices/net2grid_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::net2grid {

extern const PreparedDefinition kDef_N2G_SP;

const PreparedDefinition* const kNet2GridRegistry[] = {
    &kDef_N2G_SP,
};
const std::size_t kNet2GridRegistryCount = sizeof(kNet2GridRegistry) / sizeof(kNet2GridRegistry[0]);

}  // namespace zhc::devices::net2grid
