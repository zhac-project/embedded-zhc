// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated ihorn registry.
#include "zhc/devices/ihorn_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::ihorn {

extern const PreparedDefinition kDef_LH_09521;
extern const PreparedDefinition kDef_HO_09ZB;
extern const PreparedDefinition kDef_LH03121;
extern const PreparedDefinition kDef_LH_32ZB;
extern const PreparedDefinition kDef_LH_990F;
extern const PreparedDefinition kDef_LH_990ZB;
extern const PreparedDefinition kDef_LH_992ZB;

const PreparedDefinition* const kIhornRegistry[] = {
    &kDef_LH_09521,
    &kDef_HO_09ZB,
    &kDef_LH03121,
    &kDef_LH_32ZB,
    &kDef_LH_990F,
    &kDef_LH_990ZB,
    &kDef_LH_992ZB,
};
const std::size_t kIhornRegistryCount = sizeof(kIhornRegistry) / sizeof(kIhornRegistry[0]);

}  // namespace zhc::devices::ihorn
