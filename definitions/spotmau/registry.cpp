// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated spotmau registry.
#include "zhc/devices/spotmau_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::spotmau {

extern const PreparedDefinition kDef_SP_PS1_02;
extern const PreparedDefinition kDef_SP_PS2_02;
extern const PreparedDefinition kDef_SP_PS3_02;
extern const PreparedDefinition kDef_SP_WS_02;

const PreparedDefinition* const kSpotmauRegistry[] = {
    &kDef_SP_PS1_02,
    &kDef_SP_PS2_02,
    &kDef_SP_PS3_02,
    &kDef_SP_WS_02,
};
const std::size_t kSpotmauRegistryCount = sizeof(kSpotmauRegistry) / sizeof(kSpotmauRegistry[0]);

}  // namespace zhc::devices::spotmau
