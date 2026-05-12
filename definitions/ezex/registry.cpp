// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated ezex registry.
#include "zhc/devices/ezex_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::ezex {

extern const PreparedDefinition kDef_ECW_100_A03;

const PreparedDefinition* const kEzexRegistry[] = {
    &kDef_ECW_100_A03,
};
const std::size_t kEzexRegistryCount = sizeof(kEzexRegistry) / sizeof(kEzexRegistry[0]);

}  // namespace zhc::devices::ezex
