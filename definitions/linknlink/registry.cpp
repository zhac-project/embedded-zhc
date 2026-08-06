// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/devices/linknlink_registry.hpp"

namespace zhc::devices::linknlink {

extern const PreparedDefinition kDef_eMotion_Air;

const PreparedDefinition* const kLinknlinkRegistry[] = {
    &kDef_eMotion_Air,
};

const std::size_t kLinknlinkRegistryCount = sizeof(kLinknlinkRegistry) / sizeof(kLinknlinkRegistry[0]);

}  // namespace zhc::devices::linknlink
