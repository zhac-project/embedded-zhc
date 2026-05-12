// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated klikaanklikuit registry.
#include "zhc/devices/klikaanklikuit_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::klikaanklikuit {

extern const PreparedDefinition kDef_ZCC_3500;
extern const PreparedDefinition kDef_ZCM_1800;

const PreparedDefinition* const kKlikaanklikuitRegistry[] = {
    &kDef_ZCC_3500,
    &kDef_ZCM_1800,
};
const std::size_t kKlikaanklikuitRegistryCount = sizeof(kKlikaanklikuitRegistry) / sizeof(kKlikaanklikuitRegistry[0]);

}  // namespace zhc::devices::klikaanklikuit
