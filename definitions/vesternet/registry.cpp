// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated vesternet registry. Hand-edited 2026-04-28 to add PIR_21.
#include "zhc/devices/vesternet_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::vesternet {

extern const PreparedDefinition kDef_VES_ZB_DIM_004;
extern const PreparedDefinition kDef_VES_ZB_HLD_017;
extern const PreparedDefinition kDef_VES_ZB_MOT_019;
extern const PreparedDefinition kDef_VES_ZB_PIR_21;
extern const PreparedDefinition kDef_VES_ZB_REM_013;
extern const PreparedDefinition kDef_VES_ZB_SWI_005;
extern const PreparedDefinition kDef_VES_ZB_SWI_015;
extern const PreparedDefinition kDef_VES_ZB_WAL_006;
extern const PreparedDefinition kDef_VES_ZB_WAL_011;
extern const PreparedDefinition kDef_VES_ZB_WAL_012;

const PreparedDefinition* const kVesternetRegistry[] = {
    &kDef_VES_ZB_DIM_004,
    &kDef_VES_ZB_HLD_017,
    &kDef_VES_ZB_MOT_019,
    &kDef_VES_ZB_PIR_21,
    &kDef_VES_ZB_REM_013,
    &kDef_VES_ZB_SWI_005,
    &kDef_VES_ZB_SWI_015,
    &kDef_VES_ZB_WAL_006,
    &kDef_VES_ZB_WAL_011,
    &kDef_VES_ZB_WAL_012,
};
const std::size_t kVesternetRegistryCount = sizeof(kVesternetRegistry) / sizeof(kVesternetRegistry[0]);

}  // namespace zhc::devices::vesternet
