// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated dqsmart registry.
#include "zhc/devices/dqsmart_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::dqsmart {

extern const PreparedDefinition kDef_dqhome_re4;

const PreparedDefinition* const kDqsmartRegistry[] = {
    &kDef_dqhome_re4,
};
const std::size_t kDqsmartRegistryCount = sizeof(kDqsmartRegistry) / sizeof(kDqsmartRegistry[0]);

}  // namespace zhc::devices::dqsmart
