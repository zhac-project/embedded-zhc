// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// mowe registry.
#include "zhc/devices/mowe_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::mowe {

extern const PreparedDefinition kDef_MW833P;

const PreparedDefinition* const kMoweRegistry[] = {
    &kDef_MW833P,
};
const std::size_t kMoweRegistryCount = sizeof(kMoweRegistry) / sizeof(kMoweRegistry[0]);

}  // namespace zhc::devices::mowe
