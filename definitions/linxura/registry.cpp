// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated linxura registry.
#include "zhc/devices/linxura_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::linxura {

extern const PreparedDefinition kDef_SCHA_1_MO;
extern const PreparedDefinition kDef_SHCB_1_MO;

const PreparedDefinition* const kLinxuraRegistry[] = {
    &kDef_SCHA_1_MO,
    &kDef_SHCB_1_MO,
};
const std::size_t kLinxuraRegistryCount = sizeof(kLinxuraRegistry) / sizeof(kLinxuraRegistry[0]);

}  // namespace zhc::devices::linxura
