// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated ubisys registry.
#include "zhc/devices/ubisys_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::ubisys {

extern const PreparedDefinition kDef_C4;
extern const PreparedDefinition kDef_D1;
extern const PreparedDefinition kDef_H1;
extern const PreparedDefinition kDef_H10;
extern const PreparedDefinition kDef_J1;
extern const PreparedDefinition kDef_R0;
extern const PreparedDefinition kDef_S1;
extern const PreparedDefinition kDef_S1_R;
extern const PreparedDefinition kDef_S2;

const PreparedDefinition* const kUbisysRegistry[] = {
    &kDef_C4,
    &kDef_D1,
    &kDef_H1,
    &kDef_H10,
    &kDef_J1,
    &kDef_R0,
    &kDef_S1,
    &kDef_S1_R,
    &kDef_S2,
};
const std::size_t kUbisysRegistryCount = sizeof(kUbisysRegistry) / sizeof(kUbisysRegistry[0]);

}  // namespace zhc::devices::ubisys
