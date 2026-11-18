// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/devices/edflab_registry.hpp"

namespace zhc::devices::edflab {

extern const PreparedDefinition kDef_ZIGBEE_ERL;

const PreparedDefinition* const kEdflabRegistry[] = {
    &kDef_ZIGBEE_ERL,
};

const std::size_t kEdflabRegistryCount = sizeof(kEdflabRegistry) / sizeof(kEdflabRegistry[0]);

}  // namespace zhc::devices::edflab
