// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated zipato registry.
#include "zhc/devices/zipato_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::zipato {

extern const PreparedDefinition kDef_rgbw2_zbee27;

const PreparedDefinition* const kZipatoRegistry[] = {
    &kDef_rgbw2_zbee27,
};
const std::size_t kZipatoRegistryCount = sizeof(kZipatoRegistry) / sizeof(kZipatoRegistry[0]);

}  // namespace zhc::devices::zipato
