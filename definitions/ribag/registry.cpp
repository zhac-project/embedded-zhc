// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated ribag registry.
#include "zhc/devices/ribag_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::ribag {

extern const PreparedDefinition kDef_AirO;

const PreparedDefinition* const kRibagRegistry[] = {
    &kDef_AirO,
};
const std::size_t kRibagRegistryCount = sizeof(kRibagRegistry) / sizeof(kRibagRegistry[0]);

}  // namespace zhc::devices::ribag
