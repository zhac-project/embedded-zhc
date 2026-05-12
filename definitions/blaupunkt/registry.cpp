// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated blaupunkt registry.
#include "zhc/devices/blaupunkt_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::blaupunkt {

extern const PreparedDefinition kDef_SCM_S1;

const PreparedDefinition* const kBlaupunktRegistry[] = {
    &kDef_SCM_S1,
};
const std::size_t kBlaupunktRegistryCount = sizeof(kBlaupunktRegistry) / sizeof(kBlaupunktRegistry[0]);

}  // namespace zhc::devices::blaupunkt
