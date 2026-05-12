// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated istar registry.
#include "zhc/devices/istar_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::istar {

extern const PreparedDefinition kDef_SCCV2401_1;
extern const PreparedDefinition kDef_SCCV2401_4;
extern const PreparedDefinition kDef_SCCV2403_2;

const PreparedDefinition* const kIstarRegistry[] = {
    &kDef_SCCV2401_1,
    &kDef_SCCV2401_4,
    &kDef_SCCV2403_2,
};
const std::size_t kIstarRegistryCount = sizeof(kIstarRegistry) / sizeof(kIstarRegistry[0]);

}  // namespace zhc::devices::istar
