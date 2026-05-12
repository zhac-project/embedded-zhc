// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated vrey registry.
#include "zhc/devices/vrey_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::vrey {

extern const PreparedDefinition kDef_VR_X701U;

const PreparedDefinition* const kVreyRegistry[] = {
    &kDef_VR_X701U,
};
const std::size_t kVreyRegistryCount = sizeof(kVreyRegistry) / sizeof(kVreyRegistry[0]);

}  // namespace zhc::devices::vrey
