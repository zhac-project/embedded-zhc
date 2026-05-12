// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated halemeier registry.
#include "zhc/devices/halemeier_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::halemeier {

extern const PreparedDefinition kDef_HA_ZBM_MW2;
extern const PreparedDefinition kDef_HA_ZGMW2_E;
extern const PreparedDefinition kDef_HA_ZM12_24_1K;
extern const PreparedDefinition kDef_HA_ZM12_24_4K;
extern const PreparedDefinition kDef_HA_ZM12_24_mw2;
extern const PreparedDefinition kDef_HA_ZM12mw2_4K;
extern const PreparedDefinition kDef_HA_ZSM_MW2;
extern const PreparedDefinition kDef_HA_ZX1;

const PreparedDefinition* const kHalemeierRegistry[] = {
    &kDef_HA_ZBM_MW2,
    &kDef_HA_ZGMW2_E,
    &kDef_HA_ZM12_24_1K,
    &kDef_HA_ZM12_24_4K,
    &kDef_HA_ZM12_24_mw2,
    &kDef_HA_ZM12mw2_4K,
    &kDef_HA_ZSM_MW2,
    &kDef_HA_ZX1,
};
const std::size_t kHalemeierRegistryCount = sizeof(kHalemeierRegistry) / sizeof(kHalemeierRegistry[0]);

}  // namespace zhc::devices::halemeier
