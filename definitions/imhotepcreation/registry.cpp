// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated imhotepcreation registry.
#include "zhc/devices/imhotepcreation_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::imhotepcreation {

extern const PreparedDefinition kDef_BRI4P;
extern const PreparedDefinition kDef_E_Ctrl;

const PreparedDefinition* const kImhotepcreationRegistry[] = {
    &kDef_BRI4P,
    &kDef_E_Ctrl,
};
const std::size_t kImhotepcreationRegistryCount = sizeof(kImhotepcreationRegistry) / sizeof(kImhotepcreationRegistry[0]);

}  // namespace zhc::devices::imhotepcreation
