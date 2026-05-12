// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated elko registry.
#include "zhc/devices/elko_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::elko {

extern const PreparedDefinition kDef_D316GLEDRF;
extern const PreparedDefinition kDef_EKO05806;
extern const PreparedDefinition kDef_D4523430;

const PreparedDefinition* const kElkoRegistry[] = {
    &kDef_D316GLEDRF,
    &kDef_EKO05806,
    &kDef_D4523430,
};
const std::size_t kElkoRegistryCount = sizeof(kElkoRegistry) / sizeof(kElkoRegistry[0]);

}  // namespace zhc::devices::elko
