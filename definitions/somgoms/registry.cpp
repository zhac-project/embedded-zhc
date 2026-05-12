// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated somgoms registry.
#include "zhc/devices/somgoms_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::somgoms {

extern const PreparedDefinition kDef_SM_1CTW_EU;
extern const PreparedDefinition kDef_ZSTY_SM_11ZG_US_W;
extern const PreparedDefinition kDef_ZSTY_SM_1CTZG_US_W;
extern const PreparedDefinition kDef_ZSTY_SM_1DMZG_US_W;

const PreparedDefinition* const kSomgomsRegistry[] = {
    &kDef_SM_1CTW_EU,
    &kDef_ZSTY_SM_11ZG_US_W,
    &kDef_ZSTY_SM_1CTZG_US_W,
    &kDef_ZSTY_SM_1DMZG_US_W,
};
const std::size_t kSomgomsRegistryCount = sizeof(kSomgomsRegistry) / sizeof(kSomgomsRegistry[0]);

}  // namespace zhc::devices::somgoms
