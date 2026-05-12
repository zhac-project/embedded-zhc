// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated brun_holding registry.
#include "zhc/devices/brun_holding_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::brun_holding {

extern const PreparedDefinition kDef_Fire_Fence;

const PreparedDefinition* const kBrunHoldingRegistry[] = {
    &kDef_Fire_Fence,
};
const std::size_t kBrunHoldingRegistryCount = sizeof(kBrunHoldingRegistry) / sizeof(kBrunHoldingRegistry[0]);

}  // namespace zhc::devices::brun_holding
