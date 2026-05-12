// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated atsmart registry.
#include "zhc/devices/atsmart_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::atsmart {

extern const PreparedDefinition kDef_Z6;

const PreparedDefinition* const kAtsmartRegistry[] = {
    &kDef_Z6,
};
const std::size_t kAtsmartRegistryCount = sizeof(kAtsmartRegistry) / sizeof(kAtsmartRegistry[0]);

}  // namespace zhc::devices::atsmart
