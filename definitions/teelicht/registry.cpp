// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/devices/teelicht_registry.hpp"

namespace zhc::devices::teelicht {

extern const PreparedDefinition kDef_teelicht;

const PreparedDefinition* const kTeelichtRegistry[] = {
    &kDef_teelicht,
};

const std::size_t kTeelichtRegistryCount = sizeof(kTeelichtRegistry) / sizeof(kTeelichtRegistry[0]);

}  // namespace zhc::devices::teelicht
