// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated fantem registry.
#include "zhc/devices/fantem_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::fantem {

extern const PreparedDefinition kDef_ZB006_X;
extern const PreparedDefinition kDef_ZB003_X;

const PreparedDefinition* const kFantemRegistry[] = {
    &kDef_ZB006_X,
    &kDef_ZB003_X,
};
const std::size_t kFantemRegistryCount = sizeof(kFantemRegistry) / sizeof(kFantemRegistry[0]);

}  // namespace zhc::devices::fantem
