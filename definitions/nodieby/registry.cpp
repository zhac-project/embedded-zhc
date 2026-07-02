// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// NoDieby registry.
#include "zhc/devices/nodieby_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::nodieby {

extern const PreparedDefinition kDef_ND_01;

const PreparedDefinition* const kNodiebyRegistry[] = {
    &kDef_ND_01,
};
const std::size_t kNodiebyRegistryCount = sizeof(kNodiebyRegistry) / sizeof(kNodiebyRegistry[0]);

}  // namespace zhc::devices::nodieby
