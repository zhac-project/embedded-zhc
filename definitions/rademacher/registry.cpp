// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated rademacher registry.
#include "zhc/devices/rademacher_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::rademacher {

extern const PreparedDefinition kDef_D35104001;
extern const PreparedDefinition kDef_D35144001;
extern const PreparedDefinition kDef_RDM_35274001;

const PreparedDefinition* const kRademacherRegistry[] = {
    &kDef_D35104001,
    &kDef_D35144001,
    &kDef_RDM_35274001,
};
const std::size_t kRademacherRegistryCount = sizeof(kRademacherRegistry) / sizeof(kRademacherRegistry[0]);

}  // namespace zhc::devices::rademacher
