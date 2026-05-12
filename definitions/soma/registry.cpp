// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated soma registry.
#include "zhc/devices/soma_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::soma {

extern const PreparedDefinition kDef_SmartShades3;

const PreparedDefinition* const kSomaRegistry[] = {
    &kDef_SmartShades3,
};
const std::size_t kSomaRegistryCount = sizeof(kSomaRegistry) / sizeof(kSomaRegistry[0]);

}  // namespace zhc::devices::soma
