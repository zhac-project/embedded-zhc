// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated smartenit registry.
#include "zhc/devices/smartenit_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::smartenit {

extern const PreparedDefinition kDef_D4040B;
extern const PreparedDefinition kDef_ZBHT_1;

const PreparedDefinition* const kSmartenitRegistry[] = {
    &kDef_D4040B,
    &kDef_ZBHT_1,
};
const std::size_t kSmartenitRegistryCount = sizeof(kSmartenitRegistry) / sizeof(kSmartenitRegistry[0]);

}  // namespace zhc::devices::smartenit
