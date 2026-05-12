// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated danfoss registry.
#include "zhc/devices/danfoss_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::danfoss {

extern const PreparedDefinition kDef_D014G2461;
extern const PreparedDefinition kDef_Icon;
extern const PreparedDefinition kDef_Icon2;

const PreparedDefinition* const kDanfossRegistry[] = {
    &kDef_D014G2461,
    &kDef_Icon,
    &kDef_Icon2,
};
const std::size_t kDanfossRegistryCount = sizeof(kDanfossRegistry) / sizeof(kDanfossRegistry[0]);

}  // namespace zhc::devices::danfoss
