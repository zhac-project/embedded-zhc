// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated bouffalo_lab registry.
#include "zhc/devices/bouffalo_lab_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::bouffalo_lab {

extern const PreparedDefinition kDef_RMC002;

const PreparedDefinition* const kBouffaloLabRegistry[] = {
    &kDef_RMC002,
};
const std::size_t kBouffaloLabRegistryCount = sizeof(kBouffaloLabRegistry) / sizeof(kBouffaloLabRegistry[0]);

}  // namespace zhc::devices::bouffalo_lab
