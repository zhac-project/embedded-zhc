// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated stello registry.
#include "zhc/devices/stello_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::stello {

extern const PreparedDefinition kDef_STLO_23;
extern const PreparedDefinition kDef_STLO_34;

const PreparedDefinition* const kStelloRegistry[] = {
    &kDef_STLO_23,
    &kDef_STLO_34,
};
const std::size_t kStelloRegistryCount = sizeof(kStelloRegistry) / sizeof(kStelloRegistry[0]);

}  // namespace zhc::devices::stello
