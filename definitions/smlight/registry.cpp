// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated smlight registry.
#include "zhc/devices/smlight_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::smlight {

extern const PreparedDefinition kDef_SLZB_06M;

const PreparedDefinition* const kSmlightRegistry[] = {
    &kDef_SLZB_06M,
};
const std::size_t kSmlightRegistryCount = sizeof(kSmlightRegistry) / sizeof(kSmlightRegistry[0]);

}  // namespace zhc::devices::smlight
