// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated nyce registry.
#include "zhc/devices/nyce_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::nyce {

extern const PreparedDefinition kDef_NCZ_3010;
extern const PreparedDefinition kDef_NCZ_3011_HA;
extern const PreparedDefinition kDef_NCZ_3014_HA;
extern const PreparedDefinition kDef_NCZ_3041_HA;
extern const PreparedDefinition kDef_NCZ_3043_HA;
extern const PreparedDefinition kDef_NCZ_3045_HA;

const PreparedDefinition* const kNyceRegistry[] = {
    &kDef_NCZ_3010,
    &kDef_NCZ_3011_HA,
    &kDef_NCZ_3014_HA,
    &kDef_NCZ_3041_HA,
    &kDef_NCZ_3043_HA,
    &kDef_NCZ_3045_HA,
};
const std::size_t kNyceRegistryCount = sizeof(kNyceRegistry) / sizeof(kNyceRegistry[0]);

}  // namespace zhc::devices::nyce
