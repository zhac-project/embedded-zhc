// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated bega registry.
#include "zhc/devices/bega_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::bega {

extern const PreparedDefinition kDef_D13557;
extern const PreparedDefinition kDef_D85000;

const PreparedDefinition* const kBegaRegistry[] = {
    &kDef_D13557,
    &kDef_D85000,
};
const std::size_t kBegaRegistryCount = sizeof(kBegaRegistry) / sizeof(kBegaRegistry[0]);

}  // namespace zhc::devices::bega
