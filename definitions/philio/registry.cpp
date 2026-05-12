// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated philio registry.
#include "zhc/devices/philio_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::philio {

extern const PreparedDefinition kDef_PAT04_A;

const PreparedDefinition* const kPhilioRegistry[] = {
    &kDef_PAT04_A,
};
const std::size_t kPhilioRegistryCount = sizeof(kPhilioRegistry) / sizeof(kPhilioRegistry[0]);

}  // namespace zhc::devices::philio
