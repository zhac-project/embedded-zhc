// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated echostar registry.
#include "zhc/devices/echostar_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::echostar {

extern const PreparedDefinition kDef_SAGE206611;
extern const PreparedDefinition kDef_SAGE206612;

const PreparedDefinition* const kEchostarRegistry[] = {
    &kDef_SAGE206611,
    &kDef_SAGE206612,
};
const std::size_t kEchostarRegistryCount = sizeof(kEchostarRegistry) / sizeof(kEchostarRegistry[0]);

}  // namespace zhc::devices::echostar
