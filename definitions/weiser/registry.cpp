// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated weiser registry.
#include "zhc/devices/weiser_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::weiser {

extern const PreparedDefinition kDef_D9GED18000_009;
extern const PreparedDefinition kDef_D9GED21500_005;

const PreparedDefinition* const kWeiserRegistry[] = {
    &kDef_D9GED18000_009,
    &kDef_D9GED21500_005,
};
const std::size_t kWeiserRegistryCount = sizeof(kWeiserRegistry) / sizeof(kWeiserRegistry[0]);

}  // namespace zhc::devices::weiser
