// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated cleverio registry.
#include "zhc/devices/cleverio_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::cleverio {

extern const PreparedDefinition kDef_SA100;
extern const PreparedDefinition kDef_SS300;

const PreparedDefinition* const kCleverioRegistry[] = {
    &kDef_SA100,
    &kDef_SS300,
};
const std::size_t kCleverioRegistryCount = sizeof(kCleverioRegistry) / sizeof(kCleverioRegistry[0]);

}  // namespace zhc::devices::cleverio
