// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated m_elec registry.
#include "zhc/devices/m_elec_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::m_elec {

extern const PreparedDefinition kDef_ML_ST_BP_DIM;
extern const PreparedDefinition kDef_ML_ST_D200;
extern const PreparedDefinition kDef_ML_ST_D200_NF;
extern const PreparedDefinition kDef_ML_ST_R200;

const PreparedDefinition* const kMElecRegistry[] = {
    &kDef_ML_ST_BP_DIM,
    &kDef_ML_ST_D200,
    &kDef_ML_ST_D200_NF,
    &kDef_ML_ST_R200,
};
const std::size_t kMElecRegistryCount = sizeof(kMElecRegistry) / sizeof(kMElecRegistry[0]);

}  // namespace zhc::devices::m_elec
