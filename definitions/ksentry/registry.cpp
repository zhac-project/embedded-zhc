// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated ksentry registry.
#include "zhc/devices/ksentry_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::ksentry {

extern const PreparedDefinition kDef_KS_SM001;

const PreparedDefinition* const kKsentryRegistry[] = {
    &kDef_KS_SM001,
};
const std::size_t kKsentryRegistryCount = sizeof(kKsentryRegistry) / sizeof(kKsentryRegistry[0]);

}  // namespace zhc::devices::ksentry
