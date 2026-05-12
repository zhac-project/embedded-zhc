// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated jasco registry.
#include "zhc/devices/jasco_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::jasco {

extern const PreparedDefinition kDef_D43095;
extern const PreparedDefinition kDef_D43132;
extern const PreparedDefinition kDef_ZB3102;

const PreparedDefinition* const kJascoRegistry[] = {
    &kDef_D43095,
    &kDef_D43132,
    &kDef_ZB3102,
};
const std::size_t kJascoRegistryCount = sizeof(kJascoRegistry) / sizeof(kJascoRegistry[0]);

}  // namespace zhc::devices::jasco
