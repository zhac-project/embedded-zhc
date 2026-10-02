// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// allesin registry.
#include "zhc/devices/allesin_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::allesin {

extern const PreparedDefinition kDef_allesin_cover;

const PreparedDefinition* const kAllesinRegistry[] = {
    &kDef_allesin_cover,
};
const std::size_t kAllesinRegistryCount = sizeof(kAllesinRegistry) / sizeof(kAllesinRegistry[0]);

}  // namespace zhc::devices::allesin
