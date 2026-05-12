// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated cigol registry.
#include "zhc/devices/cigol_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::cigol {

extern const PreparedDefinition kDef_Cigol_Connect;

const PreparedDefinition* const kCigolRegistry[] = {
    &kDef_Cigol_Connect,
};
const std::size_t kCigolRegistryCount = sizeof(kCigolRegistry) / sizeof(kCigolRegistry[0]);

}  // namespace zhc::devices::cigol
