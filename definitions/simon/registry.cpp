// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated simon registry.
#include "zhc/devices/simon_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::simon {

extern const PreparedDefinition kDef_SM0501;
extern const PreparedDefinition kDef_SM0502;

const PreparedDefinition* const kSimonRegistry[] = {
    &kDef_SM0501,
    &kDef_SM0502,
};
const std::size_t kSimonRegistryCount = sizeof(kSimonRegistry) / sizeof(kSimonRegistry[0]);

}  // namespace zhc::devices::simon
