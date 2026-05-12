// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated amina registry.
#include "zhc/devices/amina_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::amina {

extern const PreparedDefinition kDef_amina_S;

const PreparedDefinition* const kAminaRegistry[] = {
    &kDef_amina_S,
};
const std::size_t kAminaRegistryCount = sizeof(kAminaRegistry) / sizeof(kAminaRegistry[0]);

}  // namespace zhc::devices::amina
