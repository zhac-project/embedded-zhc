// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated atlantic registry.
#include "zhc/devices/atlantic_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::atlantic {

extern const PreparedDefinition kDef_GW003_AS_IN_TE_FC;

const PreparedDefinition* const kAtlanticRegistry[] = {
    &kDef_GW003_AS_IN_TE_FC,
};
const std::size_t kAtlanticRegistryCount = sizeof(kAtlanticRegistry) / sizeof(kAtlanticRegistry[0]);

}  // namespace zhc::devices::atlantic
