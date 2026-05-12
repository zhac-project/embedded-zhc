// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated bseed registry.
#include "zhc/devices/bseed_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::bseed {

extern const PreparedDefinition kDef_BSEED_TS0601_cover;

const PreparedDefinition* const kBseedRegistry[] = {
    &kDef_BSEED_TS0601_cover,
};
const std::size_t kBseedRegistryCount = sizeof(kBseedRegistry) / sizeof(kBseedRegistry[0]);

}  // namespace zhc::devices::bseed
