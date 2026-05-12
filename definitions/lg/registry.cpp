// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated lg registry.
#include "zhc/devices/lg_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::lg {

extern const PreparedDefinition kDef_B1027EB0Z01;
extern const PreparedDefinition kDef_B1027EB0Z02;
extern const PreparedDefinition kDef_B1027EB4Z01;

const PreparedDefinition* const kLgRegistry[] = {
    &kDef_B1027EB0Z01,
    &kDef_B1027EB0Z02,
    &kDef_B1027EB4Z01,
};
const std::size_t kLgRegistryCount = sizeof(kLgRegistry) / sizeof(kLgRegistry[0]);

}  // namespace zhc::devices::lg
