// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated waxman registry.
#include "zhc/devices/waxman_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::waxman {

extern const PreparedDefinition kDef_D8850100;
extern const PreparedDefinition kDef_D8840100H;

const PreparedDefinition* const kWaxmanRegistry[] = {
    &kDef_D8850100,
    &kDef_D8840100H,
};
const std::size_t kWaxmanRegistryCount = sizeof(kWaxmanRegistry) / sizeof(kWaxmanRegistry[0]);

}  // namespace zhc::devices::waxman
