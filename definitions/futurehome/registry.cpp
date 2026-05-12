// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated futurehome registry.
#include "zhc/devices/futurehome_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::futurehome {

extern const PreparedDefinition kDefFut__TZE200_4hbx5cvx;
extern const PreparedDefinition kDefFut__TZE200_e5hpkc6d;
extern const PreparedDefinition kDefFut__TZE204_e5hpkc6d;
extern const PreparedDefinition kDef_D4509243;
extern const PreparedDefinition kDef_TS0601_futurehome_thermostat;

const PreparedDefinition* const kFuturehomeRegistry[] = {
    &kDefFut__TZE200_4hbx5cvx,
    &kDefFut__TZE200_e5hpkc6d,
    &kDefFut__TZE204_e5hpkc6d,
    &kDef_D4509243,
    &kDef_TS0601_futurehome_thermostat,
};
const std::size_t kFuturehomeRegistryCount = sizeof(kFuturehomeRegistry) / sizeof(kFuturehomeRegistry[0]);

}  // namespace zhc::devices::futurehome
