// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated weten registry.
#include "zhc/devices/weten_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::weten {

extern const PreparedDefinition kDefWet__TZE204_6fk3gewc;
extern const PreparedDefinition kDefWet__TZE284_6fk3gewc;
extern const PreparedDefinition kDef_D1GNNTS;
extern const PreparedDefinition kDef_PCI_E;

const PreparedDefinition* const kWetenRegistry[] = {
    &kDefWet__TZE204_6fk3gewc,
    &kDefWet__TZE284_6fk3gewc,
    &kDef_D1GNNTS,
    &kDef_PCI_E,
};
const std::size_t kWetenRegistryCount = sizeof(kWetenRegistry) / sizeof(kWetenRegistry[0]);

}  // namespace zhc::devices::weten
