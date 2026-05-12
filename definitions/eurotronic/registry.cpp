// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated eurotronic registry.
#include "zhc/devices/eurotronic_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::eurotronic {

extern const PreparedDefinition kDef_COZB0001;
extern const PreparedDefinition kDef_CoZB_dha;
extern const PreparedDefinition kDef_SPZB0001;

const PreparedDefinition* const kEurotronicRegistry[] = {
    &kDef_COZB0001,
    &kDef_CoZB_dha,
    &kDef_SPZB0001,
};
const std::size_t kEurotronicRegistryCount = sizeof(kEurotronicRegistry) / sizeof(kEurotronicRegistry[0]);

}  // namespace zhc::devices::eurotronic
