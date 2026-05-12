// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated kurvia registry.
#include "zhc/devices/kurvia_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::kurvia {

extern const PreparedDefinition kDef_ZB_CL01;

const PreparedDefinition* const kKurviaRegistry[] = {
    &kDef_ZB_CL01,
};
const std::size_t kKurviaRegistryCount = sizeof(kKurviaRegistry) / sizeof(kKurviaRegistry[0]);

}  // namespace zhc::devices::kurvia
