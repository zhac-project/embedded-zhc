// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated alchemy registry.
#include "zhc/devices/alchemy_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::alchemy {

extern const PreparedDefinition kDef_AL8RGB13W_AP;
extern const PreparedDefinition kDef_AL8TC13W_AP;

const PreparedDefinition* const kAlchemyRegistry[] = {
    &kDef_AL8RGB13W_AP,
    &kDef_AL8TC13W_AP,
};
const std::size_t kAlchemyRegistryCount = sizeof(kAlchemyRegistry) / sizeof(kAlchemyRegistry[0]);

}  // namespace zhc::devices::alchemy
