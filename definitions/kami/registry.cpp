// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated kami registry.
#include "zhc/devices/kami_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::kami {

extern const PreparedDefinition kDef_N20;

const PreparedDefinition* const kKamiRegistry[] = {
    &kDef_N20,
};
const std::size_t kKamiRegistryCount = sizeof(kKamiRegistry) / sizeof(kKamiRegistry[0]);

}  // namespace zhc::devices::kami
