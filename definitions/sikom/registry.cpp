// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated sikom registry.
#include "zhc/devices/sikom_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::sikom {

extern const PreparedDefinition kDef_D300_9715V10;

const PreparedDefinition* const kSikomRegistry[] = {
    &kDef_D300_9715V10,
};
const std::size_t kSikomRegistryCount = sizeof(kSikomRegistry) / sizeof(kSikomRegistry[0]);

}  // namespace zhc::devices::sikom
