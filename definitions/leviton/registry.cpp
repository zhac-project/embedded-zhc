// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated leviton registry.
#include "zhc/devices/leviton_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::leviton {

extern const PreparedDefinition kDef_DG15A_1BW;
extern const PreparedDefinition kDef_DG15S_1BW;
extern const PreparedDefinition kDef_DG3HL_1BW;
extern const PreparedDefinition kDef_DG6HD_1BW;
extern const PreparedDefinition kDef_DL15S_1BZ;
extern const PreparedDefinition kDef_RC_2000WH;
extern const PreparedDefinition kDef_ZS057_D0Z;

const PreparedDefinition* const kLevitonRegistry[] = {
    &kDef_DG15A_1BW,
    &kDef_DG15S_1BW,
    &kDef_DG3HL_1BW,
    &kDef_DG6HD_1BW,
    &kDef_DL15S_1BZ,
    &kDef_RC_2000WH,
    &kDef_ZS057_D0Z,
};
const std::size_t kLevitonRegistryCount = sizeof(kLevitonRegistry) / sizeof(kLevitonRegistry[0]);

}  // namespace zhc::devices::leviton
