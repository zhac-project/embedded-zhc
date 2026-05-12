// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated plugwise registry.
#include "zhc/devices/plugwise_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::plugwise {

extern const PreparedDefinition kDef_D106_03;
extern const PreparedDefinition kDef_D160_01;
extern const PreparedDefinition kDef_D158_01;
extern const PreparedDefinition kDef_D170_01;

const PreparedDefinition* const kPlugwiseRegistry[] = {
    &kDef_D106_03,
    &kDef_D160_01,
    &kDef_D158_01,
    &kDef_D170_01,
};
const std::size_t kPlugwiseRegistryCount = sizeof(kPlugwiseRegistry) / sizeof(kPlugwiseRegistry[0]);

}  // namespace zhc::devices::plugwise
