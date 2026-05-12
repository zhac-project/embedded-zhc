// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated nedis registry.
#include "zhc/devices/nedis_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::nedis {

extern const PreparedDefinition kDefNed__TZE200_ne4pikwm;
extern const PreparedDefinition kDefNed__TZE284_hcs66axl;
extern const PreparedDefinition kDefNed__TZE284_ne4pikwm;
extern const PreparedDefinition kDef_ZBHTR20WT;

const PreparedDefinition* const kNedisRegistry[] = {
    &kDefNed__TZE200_ne4pikwm,
    &kDefNed__TZE284_hcs66axl,
    &kDefNed__TZE284_ne4pikwm,
    &kDef_ZBHTR20WT,
};
const std::size_t kNedisRegistryCount = sizeof(kNedisRegistry) / sizeof(kNedisRegistry[0]);

}  // namespace zhc::devices::nedis
