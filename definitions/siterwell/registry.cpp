// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated siterwell registry.
#include "zhc/devices/siterwell_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::siterwell {

extern const PreparedDefinition kDef_GS361A_H04;

const PreparedDefinition* const kSiterwellRegistry[] = {
    &kDef_GS361A_H04,
};
const std::size_t kSiterwellRegistryCount = sizeof(kSiterwellRegistry) / sizeof(kSiterwellRegistry[0]);

}  // namespace zhc::devices::siterwell
