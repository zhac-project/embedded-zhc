// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated enocean registry.
#include "zhc/devices/enocean_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::enocean {

extern const PreparedDefinition kDef_PTM_215Z;
extern const PreparedDefinition kDef_PTM_215ZE;
extern const PreparedDefinition kDef_PTM_216Z;

const PreparedDefinition* const kEnoceanRegistry[] = {
    &kDef_PTM_215Z,
    &kDef_PTM_215ZE,
    &kDef_PTM_216Z,
};
const std::size_t kEnoceanRegistryCount = sizeof(kEnoceanRegistry) / sizeof(kEnoceanRegistry[0]);

}  // namespace zhc::devices::enocean
