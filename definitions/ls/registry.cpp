// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated ls registry.
#include "zhc/devices/ls_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::ls {

extern const PreparedDefinition kDef_A319463;
extern const PreparedDefinition kDef_D756200027;
extern const PreparedDefinition kDef_D756200030;
extern const PreparedDefinition kDef_D756200643;

const PreparedDefinition* const kLsRegistry[] = {
    &kDef_A319463,
    &kDef_D756200027,
    &kDef_D756200030,
    &kDef_D756200643,
};
const std::size_t kLsRegistryCount = sizeof(kLsRegistry) / sizeof(kLsRegistry[0]);

}  // namespace zhc::devices::ls
