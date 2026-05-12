// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated danalock registry.
#include "zhc/devices/danalock_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::danalock {

extern const PreparedDefinition kDef_V3_BTZB_V3_BTZBE;

const PreparedDefinition* const kDanalockRegistry[] = {
    &kDef_V3_BTZB_V3_BTZBE,
};
const std::size_t kDanalockRegistryCount = sizeof(kDanalockRegistry) / sizeof(kDanalockRegistry[0]);

}  // namespace zhc::devices::danalock
