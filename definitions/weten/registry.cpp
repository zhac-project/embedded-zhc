// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated weten registry.
#include "zhc/devices/weten_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::weten {

extern const PreparedDefinition kDef_PCI_E;
extern const PreparedDefinition kDef_D1GNNTS;

const PreparedDefinition* const kWetenRegistry[] = {
    &kDef_PCI_E,
    &kDef_D1GNNTS,
};
const std::size_t kWetenRegistryCount = sizeof(kWetenRegistry) / sizeof(kWetenRegistry[0]);

}  // namespace zhc::devices::weten
