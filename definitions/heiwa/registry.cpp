// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// heiwa registry.
#include "zhc/devices/heiwa_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::heiwa {

extern const PreparedDefinition kDef_HPZERAD_V1;

const PreparedDefinition* const kHeiwaRegistry[] = {
    &kDef_HPZERAD_V1,
};
const std::size_t kHeiwaRegistryCount = sizeof(kHeiwaRegistry) / sizeof(kHeiwaRegistry[0]);

}  // namespace zhc::devices::heiwa
