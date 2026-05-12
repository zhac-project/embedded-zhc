// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated cleode registry.
#include "zhc/devices/cleode_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::cleode {

extern const PreparedDefinition kDef_ZPLUG_Boost;

const PreparedDefinition* const kCleodeRegistry[] = {
    &kDef_ZPLUG_Boost,
};
const std::size_t kCleodeRegistryCount = sizeof(kCleodeRegistry) / sizeof(kCleodeRegistry[0]);

}  // namespace zhc::devices::cleode
