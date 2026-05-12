// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated solaredge registry.
#include "zhc/devices/solaredge_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::solaredge {

extern const PreparedDefinition kDef_SEHAZB_DR_SWITCH_2;

const PreparedDefinition* const kSolaredgeRegistry[] = {
    &kDef_SEHAZB_DR_SWITCH_2,
};
const std::size_t kSolaredgeRegistryCount = sizeof(kSolaredgeRegistry) / sizeof(kSolaredgeRegistry[0]);

}  // namespace zhc::devices::solaredge
