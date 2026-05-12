// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated kmpcil registry.
#include "zhc/devices/kmpcil_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::kmpcil {

extern const PreparedDefinition kDef_KMPCIL_RES005;
extern const PreparedDefinition kDef_KMPCIL_tag_001;

const PreparedDefinition* const kKmpcilRegistry[] = {
    &kDef_KMPCIL_RES005,
    &kDef_KMPCIL_tag_001,
};
const std::size_t kKmpcilRegistryCount = sizeof(kKmpcilRegistry) / sizeof(kKmpcilRegistry[0]);

}  // namespace zhc::devices::kmpcil
