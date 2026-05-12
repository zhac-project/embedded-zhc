// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated inovelli registry.
#include "zhc/devices/inovelli_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::inovelli {

extern const PreparedDefinition kDef_VZM30_SN;
extern const PreparedDefinition kDef_VZM31_SN;
extern const PreparedDefinition kDef_VZM32_SN;
extern const PreparedDefinition kDef_VZM36;
extern const PreparedDefinition kDef_VZM35_SN;

const PreparedDefinition* const kInovelliRegistry[] = {
    &kDef_VZM30_SN,
    &kDef_VZM31_SN,
    &kDef_VZM32_SN,
    &kDef_VZM36,
    &kDef_VZM35_SN,
};
const std::size_t kInovelliRegistryCount = sizeof(kInovelliRegistry) / sizeof(kInovelliRegistry[0]);

}  // namespace zhc::devices::inovelli
