// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/devices/heatzy_registry.hpp"

namespace zhc::devices::heatzy {

extern const PreparedDefinition kDef_micro_duo;

const PreparedDefinition* const kHeatzyRegistry[] = {
    &kDef_micro_duo,
};

const std::size_t kHeatzyRegistryCount = sizeof(kHeatzyRegistry) / sizeof(kHeatzyRegistry[0]);

}  // namespace zhc::devices::heatzy
