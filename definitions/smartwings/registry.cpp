// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated smartwings registry.
#include "zhc/devices/smartwings_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::smartwings {

extern const PreparedDefinition kDef_WM25L_Z;

const PreparedDefinition* const kSmartwingsRegistry[] = {
    &kDef_WM25L_Z,
};
const std::size_t kSmartwingsRegistryCount = sizeof(kSmartwingsRegistry) / sizeof(kSmartwingsRegistry[0]);

}  // namespace zhc::devices::smartwings
