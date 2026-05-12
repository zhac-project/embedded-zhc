// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated insta registry.
#include "zhc/devices/insta_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::insta {

extern const PreparedDefinition kDef_D57004000;
extern const PreparedDefinition kDef_D57005000;
extern const PreparedDefinition kDef_D57008000;
extern const PreparedDefinition kDef_InstaRemote;

const PreparedDefinition* const kInstaRegistry[] = {
    &kDef_D57004000,
    &kDef_D57005000,
    &kDef_D57008000,
    &kDef_InstaRemote,
};
const std::size_t kInstaRegistryCount = sizeof(kInstaRegistry) / sizeof(kInstaRegistry[0]);

}  // namespace zhc::devices::insta
