// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated quotra registry.
#include "zhc/devices/quotra_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::quotra {

extern const PreparedDefinition kDef_B07CVL9SZF;
extern const PreparedDefinition kDef_B07JHL6DRV;

const PreparedDefinition* const kQuotraRegistry[] = {
    &kDef_B07CVL9SZF,
    &kDef_B07JHL6DRV,
};
const std::size_t kQuotraRegistryCount = sizeof(kQuotraRegistry) / sizeof(kQuotraRegistry[0]);

}  // namespace zhc::devices::quotra
