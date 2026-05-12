// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated mill registry.
#include "zhc/devices/mill_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::mill {

extern const PreparedDefinition kDef_MFTWIFI;
extern const PreparedDefinition kDef_Mill_gen_4;

const PreparedDefinition* const kMillRegistry[] = {
    &kDef_MFTWIFI,
    &kDef_Mill_gen_4,
};
const std::size_t kMillRegistryCount = sizeof(kMillRegistry) / sizeof(kMillRegistry[0]);

}  // namespace zhc::devices::mill
