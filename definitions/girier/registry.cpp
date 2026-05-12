// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated girier registry.
#include "zhc/devices/girier_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::girier {

extern const PreparedDefinition kDef_JR_ZDS01;

const PreparedDefinition* const kGirierRegistry[] = {
    &kDef_JR_ZDS01,
};
const std::size_t kGirierRegistryCount = sizeof(kGirierRegistry) / sizeof(kGirierRegistry[0]);

}  // namespace zhc::devices::girier
