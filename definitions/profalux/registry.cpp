// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated profalux registry.
#include "zhc/devices/profalux_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::profalux {

extern const PreparedDefinition kDef_MAI_ZTM20C;
extern const PreparedDefinition kDef_MOT_C1ZxxC_F;
extern const PreparedDefinition kDef_MAI_ZTP20;
extern const PreparedDefinition kDef_NB102;
extern const PreparedDefinition kDef_NSAV061;

const PreparedDefinition* const kProfaluxRegistry[] = {
    &kDef_MAI_ZTM20C,
    &kDef_MOT_C1ZxxC_F,
    &kDef_MAI_ZTP20,
    &kDef_NB102,
    &kDef_NSAV061,
};
const std::size_t kProfaluxRegistryCount = sizeof(kProfaluxRegistry) / sizeof(kProfaluxRegistry[0]);

}  // namespace zhc::devices::profalux
