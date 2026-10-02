// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated atlantic registry.
#include "zhc/devices/atlantic_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::atlantic {

extern const PreparedDefinition kDef_GW003_AS_IN_TE_FC;
extern const PreparedDefinition kDef_D100042838900;
extern const PreparedDefinition kDef_D100052992400;
extern const PreparedDefinition kDef_D100052994200;

const PreparedDefinition* const kAtlanticRegistry[] = {
    &kDef_GW003_AS_IN_TE_FC,
    &kDef_D100042838900,
    &kDef_D100052992400,
    &kDef_D100052994200,
};
const std::size_t kAtlanticRegistryCount = sizeof(kAtlanticRegistry) / sizeof(kAtlanticRegistry[0]);

}  // namespace zhc::devices::atlantic
