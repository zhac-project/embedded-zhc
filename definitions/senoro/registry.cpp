// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated senoro registry.
#include "zhc/devices/senoro_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::senoro {

extern const PreparedDefinition kDefSen__TZE200_ytx9fudw;
extern const PreparedDefinition kDef_Senoro_Win;

const PreparedDefinition* const kSenoroRegistry[] = {
    &kDefSen__TZE200_ytx9fudw,
    &kDef_Senoro_Win,
};
const std::size_t kSenoroRegistryCount = sizeof(kSenoroRegistry) / sizeof(kSenoroRegistry[0]);

}  // namespace zhc::devices::senoro
