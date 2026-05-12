// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated zbeacon registry.
#include "zhc/devices/zbeacon_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::zbeacon {

extern const PreparedDefinition kDef_TS0721;

const PreparedDefinition* const kZbeaconRegistry[] = {
    &kDef_TS0721,
};
const std::size_t kZbeaconRegistryCount = sizeof(kZbeaconRegistry) / sizeof(kZbeaconRegistry[0]);

}  // namespace zhc::devices::zbeacon
