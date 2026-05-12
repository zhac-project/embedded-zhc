// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated chacon registry.
#include "zhc/devices/chacon_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::chacon {

extern const PreparedDefinition kDef_ZB_ERSM_01;
extern const PreparedDefinition kDef_ZB_PM_01;

const PreparedDefinition* const kChaconRegistry[] = {
    &kDef_ZB_ERSM_01,
    &kDef_ZB_PM_01,
};
const std::size_t kChaconRegistryCount = sizeof(kChaconRegistry) / sizeof(kChaconRegistry[0]);

}  // namespace zhc::devices::chacon
