// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated alecto registry.
#include "zhc/devices/alecto_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::alecto {

extern const PreparedDefinition kDef_SMART_HEAT10;
extern const PreparedDefinition kDef_SMART_SMOKE10;

const PreparedDefinition* const kAlectoRegistry[] = {
    &kDef_SMART_HEAT10,
    &kDef_SMART_SMOKE10,
};
const std::size_t kAlectoRegistryCount = sizeof(kAlectoRegistry) / sizeof(kAlectoRegistry[0]);

}  // namespace zhc::devices::alecto
