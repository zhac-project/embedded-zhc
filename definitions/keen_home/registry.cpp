// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated keen_home registry.
#include "zhc/devices/keen_home_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::keen_home {

extern const PreparedDefinition kDef_GW01;
extern const PreparedDefinition kDef_GW02;
extern const PreparedDefinition kDef_SV01;
extern const PreparedDefinition kDef_SV02;
extern const PreparedDefinition kDef_RS_THP_MP_1_0;

const PreparedDefinition* const kKeenHomeRegistry[] = {
    &kDef_GW01,
    &kDef_GW02,
    &kDef_SV01,
    &kDef_SV02,
    &kDef_RS_THP_MP_1_0,
};
const std::size_t kKeenHomeRegistryCount = sizeof(kKeenHomeRegistry) / sizeof(kKeenHomeRegistry[0]);

}  // namespace zhc::devices::keen_home
