// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated tnce registry.
#include "zhc/devices/tnce_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::tnce {

extern const PreparedDefinition kDef_TNCE_CLIMATE;

const PreparedDefinition* const kTnceRegistry[] = {
    &kDef_TNCE_CLIMATE,
};
const std::size_t kTnceRegistryCount = sizeof(kTnceRegistry) / sizeof(kTnceRegistry[0]);

}  // namespace zhc::devices::tnce
