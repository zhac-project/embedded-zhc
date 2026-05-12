// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated vsmart registry.
#include "zhc/devices/vsmart_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::vsmart {

extern const PreparedDefinition kDef_HS_SW100ZB_VNM;
extern const PreparedDefinition kDef_HS_SW200ZB_VNM;
extern const PreparedDefinition kDef_HS_SW300ZB_VNM;
extern const PreparedDefinition kDef_HS_SW400ZB_VNM;
extern const PreparedDefinition kDef_HS_SEDR00ZB_VNM;
extern const PreparedDefinition kDef_HS_SEOC00ZB_VNM;

const PreparedDefinition* const kVsmartRegistry[] = {
    &kDef_HS_SW100ZB_VNM,
    &kDef_HS_SW200ZB_VNM,
    &kDef_HS_SW300ZB_VNM,
    &kDef_HS_SW400ZB_VNM,
    &kDef_HS_SEDR00ZB_VNM,
    &kDef_HS_SEOC00ZB_VNM,
};
const std::size_t kVsmartRegistryCount = sizeof(kVsmartRegistry) / sizeof(kVsmartRegistry[0]);

}  // namespace zhc::devices::vsmart
