// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated securifi registry.
#include "zhc/devices/securifi_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::securifi {

extern const PreparedDefinition kDef_B01M7Y8BP9;
extern const PreparedDefinition kDef_PP_WHT_US;

const PreparedDefinition* const kSecurifiRegistry[] = {
    &kDef_B01M7Y8BP9,
    &kDef_PP_WHT_US,
};
const std::size_t kSecurifiRegistryCount = sizeof(kSecurifiRegistry) / sizeof(kSecurifiRegistry[0]);

}  // namespace zhc::devices::securifi
