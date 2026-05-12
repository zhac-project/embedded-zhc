// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated sprut registry.
#include "zhc/devices/sprut_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::sprut {

extern const PreparedDefinition kDef_Bed_box;
extern const PreparedDefinition kDef_Drivent;

const PreparedDefinition* const kSprutRegistry[] = {
    &kDef_Bed_box,
    &kDef_Drivent,
};
const std::size_t kSprutRegistryCount = sizeof(kSprutRegistry) / sizeof(kSprutRegistry[0]);

}  // namespace zhc::devices::sprut
