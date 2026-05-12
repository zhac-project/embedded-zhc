// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated gewiss registry.
#include "zhc/devices/gewiss_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::gewiss {

extern const PreparedDefinition kDef_GWA1201_TWO_WAY_SWITCH;
extern const PreparedDefinition kDef_GWA1521;
extern const PreparedDefinition kDef_GWA1522;
extern const PreparedDefinition kDef_GWA1531;
extern const PreparedDefinition kDef_GWA1501;
extern const PreparedDefinition kDef_GWA1502;

const PreparedDefinition* const kGewissRegistry[] = {
    &kDef_GWA1201_TWO_WAY_SWITCH,
    &kDef_GWA1521,
    &kDef_GWA1522,
    &kDef_GWA1531,
    &kDef_GWA1501,
    &kDef_GWA1502,
};
const std::size_t kGewissRegistryCount = sizeof(kGewissRegistry) / sizeof(kGewissRegistry[0]);

}  // namespace zhc::devices::gewiss
