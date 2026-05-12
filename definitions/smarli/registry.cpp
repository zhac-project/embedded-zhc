// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated smarli registry.
#include "zhc/devices/smarli_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::smarli {

extern const PreparedDefinition kDef_S_ZB_1RE1_R251;
extern const PreparedDefinition kDef_S_ZB_COV1_R251;
extern const PreparedDefinition kDef_S_ZB_PDM1_R251;

const PreparedDefinition* const kSmarliRegistry[] = {
    &kDef_S_ZB_1RE1_R251,
    &kDef_S_ZB_COV1_R251,
    &kDef_S_ZB_PDM1_R251,
};
const std::size_t kSmarliRegistryCount = sizeof(kSmarliRegistry) / sizeof(kSmarliRegistry[0]);

}  // namespace zhc::devices::smarli
