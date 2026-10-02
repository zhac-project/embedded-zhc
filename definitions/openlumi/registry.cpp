// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated openlumi registry.
#include "zhc/devices/openlumi_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::openlumi {

extern const PreparedDefinition kDef_GWRJN5169;
extern const PreparedDefinition kDef_LR_DGNWG05LM;
extern const PreparedDefinition kDef_LR_ZHWG11LM;

const PreparedDefinition* const kOpenlumiRegistry[] = {
    &kDef_GWRJN5169,
    &kDef_LR_DGNWG05LM,
    &kDef_LR_ZHWG11LM,
};
const std::size_t kOpenlumiRegistryCount = sizeof(kOpenlumiRegistry) / sizeof(kOpenlumiRegistry[0]);

}  // namespace zhc::devices::openlumi
