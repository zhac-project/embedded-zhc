// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated tlwglobal registry.
#include "zhc/devices/tlwglobal_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::tlwglobal {

extern const PreparedDefinition kDef_K10_1220Z;
extern const PreparedDefinition kDef_K10_1230Z;
extern const PreparedDefinition kDef_K10_1250Z;

const PreparedDefinition* const kTlwglobalRegistry[] = {
    &kDef_K10_1220Z,
    &kDef_K10_1230Z,
    &kDef_K10_1250Z,
};
const std::size_t kTlwglobalRegistryCount = sizeof(kTlwglobalRegistry) / sizeof(kTlwglobalRegistry[0]);

}  // namespace zhc::devices::tlwglobal
