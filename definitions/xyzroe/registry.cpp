// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated xyzroe registry.
#include "zhc/devices/xyzroe_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::xyzroe {

extern const PreparedDefinition kDef_ZigDC;
extern const PreparedDefinition kDef_ZigUSB;
extern const PreparedDefinition kDef_ZigUSB_C6;

const PreparedDefinition* const kXyzroeRegistry[] = {
    &kDef_ZigDC,
    &kDef_ZigUSB,
    &kDef_ZigUSB_C6,
};
const std::size_t kXyzroeRegistryCount = sizeof(kXyzroeRegistry) / sizeof(kXyzroeRegistry[0]);

}  // namespace zhc::devices::xyzroe
