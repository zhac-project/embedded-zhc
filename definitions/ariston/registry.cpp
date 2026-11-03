// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/devices/ariston_registry.hpp"

namespace zhc::devices::ariston {

extern const PreparedDefinition kDef_Rem4_Heater;

const PreparedDefinition* const kAristonRegistry[] = {
    &kDef_Rem4_Heater,
};

const std::size_t kAristonRegistryCount = sizeof(kAristonRegistry) / sizeof(kAristonRegistry[0]);

}  // namespace zhc::devices::ariston
