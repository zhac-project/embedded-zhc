// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated makegood registry.
#include "zhc/devices/makegood_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::makegood {

extern const PreparedDefinition kDef_MG_AUZG01;
extern const PreparedDefinition kDef_MG_GPO01;

const PreparedDefinition* const kMakegoodRegistry[] = {
    &kDef_MG_AUZG01,
    &kDef_MG_GPO01,
};
const std::size_t kMakegoodRegistryCount = sizeof(kMakegoodRegistry) / sizeof(kMakegoodRegistry[0]);

}  // namespace zhc::devices::makegood
