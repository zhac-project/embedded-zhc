// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated miboxer registry.
#include "zhc/devices/miboxer_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::miboxer {

extern const PreparedDefinition kDef_FUT038Z;
extern const PreparedDefinition kDef_FUT089Z;

const PreparedDefinition* const kMiboxerRegistry[] = {
    &kDef_FUT038Z,
    &kDef_FUT089Z,
};
const std::size_t kMiboxerRegistryCount = sizeof(kMiboxerRegistry) / sizeof(kMiboxerRegistry[0]);

}  // namespace zhc::devices::miboxer
