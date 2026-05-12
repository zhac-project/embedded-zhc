// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated evvr registry.
#include "zhc/devices/evvr_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::evvr {

extern const PreparedDefinition kDef_SRB01;

const PreparedDefinition* const kEvvrRegistry[] = {
    &kDef_SRB01,
};
const std::size_t kEvvrRegistryCount = sizeof(kEvvrRegistry) / sizeof(kEvvrRegistry[0]);

}  // namespace zhc::devices::evvr
