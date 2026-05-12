// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated livingwise registry.
#include "zhc/devices/livingwise_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::livingwise {

extern const PreparedDefinition kDef_LVS_SC7;
extern const PreparedDefinition kDef_LVS_ZB15R;
extern const PreparedDefinition kDef_LVS_ZB15S;
extern const PreparedDefinition kDef_LVS_ZB500D;
extern const PreparedDefinition kDef_LVS_SM10ZW;
extern const PreparedDefinition kDef_LVS_SN10ZW_SN11;

const PreparedDefinition* const kLivingwiseRegistry[] = {
    &kDef_LVS_SC7,
    &kDef_LVS_ZB15R,
    &kDef_LVS_ZB15S,
    &kDef_LVS_ZB500D,
    &kDef_LVS_SM10ZW,
    &kDef_LVS_SN10ZW_SN11,
};
const std::size_t kLivingwiseRegistryCount = sizeof(kLivingwiseRegistry) / sizeof(kLivingwiseRegistry[0]);

}  // namespace zhc::devices::livingwise
