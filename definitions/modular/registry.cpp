// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated modular registry.
#include "zhc/devices/modular_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::modular {

extern const PreparedDefinition kDef_MWM002;

const PreparedDefinition* const kModularRegistry[] = {
    &kDef_MWM002,
};
const std::size_t kModularRegistryCount = sizeof(kModularRegistry) / sizeof(kModularRegistry[0]);

}  // namespace zhc::devices::modular
