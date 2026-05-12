// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated casaia registry.
#include "zhc/devices/casaia_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::casaia {

extern const PreparedDefinition kDef_CCB432;
extern const PreparedDefinition kDef_CSAC451_WTC_E;
extern const PreparedDefinition kDef_CSLC601_D_E;
extern const PreparedDefinition kDef_CTHS_317_ET;

const PreparedDefinition* const kCasaiaRegistry[] = {
    &kDef_CCB432,
    &kDef_CSAC451_WTC_E,
    &kDef_CSLC601_D_E,
    &kDef_CTHS_317_ET,
};
const std::size_t kCasaiaRegistryCount = sizeof(kCasaiaRegistry) / sizeof(kCasaiaRegistry[0]);

}  // namespace zhc::devices::casaia
