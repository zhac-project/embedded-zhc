// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated stelpro registry.
#include "zhc/devices/stelpro_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::stelpro {

extern const PreparedDefinition kDef_HT402;
extern const PreparedDefinition kDef_SMT402;
extern const PreparedDefinition kDef_SMT402AD;
extern const PreparedDefinition kDef_SORB;
extern const PreparedDefinition kDef_ST218;
extern const PreparedDefinition kDef_STZB402;

const PreparedDefinition* const kStelproRegistry[] = {
    &kDef_HT402,
    &kDef_SMT402,
    &kDef_SMT402AD,
    &kDef_SORB,
    &kDef_ST218,
    &kDef_STZB402,
};
const std::size_t kStelproRegistryCount = sizeof(kStelproRegistry) / sizeof(kStelproRegistry[0]);

}  // namespace zhc::devices::stelpro
