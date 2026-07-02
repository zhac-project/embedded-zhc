// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated ensystec registry.
#include "zhc/devices/ensystec_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::ensystec {

extern const PreparedDefinition kDef_Ensystec_Leak_Protect_Zigbee;

const PreparedDefinition* const kEnsystecRegistry[] = {
    &kDef_Ensystec_Leak_Protect_Zigbee,
};
const std::size_t kEnsystecRegistryCount = sizeof(kEnsystecRegistry) / sizeof(kEnsystecRegistry[0]);

}  // namespace zhc::devices::ensystec
