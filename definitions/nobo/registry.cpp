// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated nobo registry.
#include "zhc/devices/nobo_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::nobo {

extern const PreparedDefinition kDef_SPC_IZ;
extern const PreparedDefinition kDef_SWT_IZ;

const PreparedDefinition* const kNoboRegistry[] = {
    &kDef_SPC_IZ,
    &kDef_SWT_IZ,
};
const std::size_t kNoboRegistryCount = sizeof(kNoboRegistry) / sizeof(kNoboRegistry[0]);

}  // namespace zhc::devices::nobo
