// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated bticino registry.
#include "zhc/devices/bticino_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::bticino {

extern const PreparedDefinition kDef_D4411C_L4411C_N4411C_NT4411C;
extern const PreparedDefinition kDef_F20T60A;
extern const PreparedDefinition kDef_K4003C_L4003C_N4003C_NT4003C;
extern const PreparedDefinition kDef_L4531C;

const PreparedDefinition* const kBticinoRegistry[] = {
    &kDef_D4411C_L4411C_N4411C_NT4411C,
    &kDef_F20T60A,
    &kDef_K4003C_L4003C_N4003C_NT4003C,
    &kDef_L4531C,
};
const std::size_t kBticinoRegistryCount = sizeof(kBticinoRegistry) / sizeof(kBticinoRegistry[0]);

}  // namespace zhc::devices::bticino
