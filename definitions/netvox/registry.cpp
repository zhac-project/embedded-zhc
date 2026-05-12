// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated netvox registry.
#include "zhc/devices/netvox_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::netvox {

extern const PreparedDefinition kDef_Z809A;
extern const PreparedDefinition kDef_ZB02A;

const PreparedDefinition* const kNetvoxRegistry[] = {
    &kDef_Z809A,
    &kDef_ZB02A,
};
const std::size_t kNetvoxRegistryCount = sizeof(kNetvoxRegistry) / sizeof(kNetvoxRegistry[0]);

}  // namespace zhc::devices::netvox
