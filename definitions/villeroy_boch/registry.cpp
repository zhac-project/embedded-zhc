// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated villeroy_boch registry.
#include "zhc/devices/villeroy_boch_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::villeroy_boch {

extern const PreparedDefinition kDef_C0040000;
extern const PreparedDefinition kDef_C5850000;

const PreparedDefinition* const kVilleroyBochRegistry[] = {
    &kDef_C0040000,
    &kDef_C5850000,
};
const std::size_t kVilleroyBochRegistryCount = sizeof(kVilleroyBochRegistry) / sizeof(kVilleroyBochRegistry[0]);

}  // namespace zhc::devices::villeroy_boch
