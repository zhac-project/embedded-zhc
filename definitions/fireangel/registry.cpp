// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated fireangel registry.
#include "zhc/devices/fireangel_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::fireangel {

extern const PreparedDefinition kDef_W2_Module;
extern const PreparedDefinition kDef_ZBCO_AE_10X_EUR;
const PreparedDefinition* const kFireangelRegistry[] = {
    &kDef_ZBCO_AE_10X_EUR,   // manufacturer-gated ("Fireangel") — Pass 1
    &kDef_W2_Module,         // model-only fallback for other manufacturers
};
const std::size_t kFireangelRegistryCount = sizeof(kFireangelRegistry) / sizeof(kFireangelRegistry[0]);

}  // namespace zhc::devices::fireangel
