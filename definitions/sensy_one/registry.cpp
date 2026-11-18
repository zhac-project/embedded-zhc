// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/devices/sensy_one_registry.hpp"

namespace zhc::devices::sensy_one {

extern const PreparedDefinition kDef_AirBee;

const PreparedDefinition* const kSensyOneRegistry[] = {
    &kDef_AirBee,
};

const std::size_t kSensyOneRegistryCount = sizeof(kSensyOneRegistry) / sizeof(kSensyOneRegistry[0]);

}  // namespace zhc::devices::sensy_one
