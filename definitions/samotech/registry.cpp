// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated samotech registry.
#include "zhc/devices/samotech_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::samotech {

extern const PreparedDefinition kDef_SM308;
extern const PreparedDefinition kDef_SM308_2CH;
extern const PreparedDefinition kDef_SM308_S;
extern const PreparedDefinition kDef_SM309;
extern const PreparedDefinition kDef_SM309_S;
extern const PreparedDefinition kDef_SM309_S_2CH;
extern const PreparedDefinition kDef_SM315;
extern const PreparedDefinition kDef_SM323_v1;
extern const PreparedDefinition kDef_SM323_v2;
extern const PreparedDefinition kDef_SM324;
extern const PreparedDefinition kDef_SM325_ZG;

const PreparedDefinition* const kSamotechRegistry[] = {
    &kDef_SM308,
    &kDef_SM308_2CH,
    &kDef_SM308_S,
    &kDef_SM309,
    &kDef_SM309_S,
    &kDef_SM309_S_2CH,
    &kDef_SM315,
    &kDef_SM323_v1,
    &kDef_SM323_v2,
    &kDef_SM324,
    &kDef_SM325_ZG,
};
const std::size_t kSamotechRegistryCount = sizeof(kSamotechRegistry) / sizeof(kSamotechRegistry[0]);

}  // namespace zhc::devices::samotech
