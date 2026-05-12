// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated task_lighting registry.
#include "zhc/devices/task_lighting_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::task_lighting {

extern const PreparedDefinition kDef_T_TWAV_60W_PSR;

const PreparedDefinition* const kTaskLightingRegistry[] = {
    &kDef_T_TWAV_60W_PSR,
};
const std::size_t kTaskLightingRegistryCount = sizeof(kTaskLightingRegistry) / sizeof(kTaskLightingRegistry[0]);

}  // namespace zhc::devices::task_lighting
