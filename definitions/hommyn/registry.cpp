// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated hommyn registry.
#include "zhc/devices/hommyn_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::hommyn {

extern const PreparedDefinition kDef_MS_20_Z;
extern const PreparedDefinition kDef_WS_20_Z;

const PreparedDefinition* const kHommynRegistry[] = {
    &kDef_MS_20_Z,
    &kDef_WS_20_Z,
};
const std::size_t kHommynRegistryCount = sizeof(kHommynRegistry) / sizeof(kHommynRegistry[0]);

}  // namespace zhc::devices::hommyn
