// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated frankever registry.
#include "zhc/devices/frankever_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::frankever {

extern const PreparedDefinition kDef_FK_V02;

const PreparedDefinition* const kFrankeverRegistry[] = {
    &kDef_FK_V02,
};
const std::size_t kFrankeverRegistryCount = sizeof(kFrankeverRegistry) / sizeof(kFrankeverRegistry[0]);

}  // namespace zhc::devices::frankever
