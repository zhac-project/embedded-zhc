// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated meazon registry.
#include "zhc/devices/meazon_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::meazon {

extern const PreparedDefinition kDef_MEAZON_BIZY_PLUG;
extern const PreparedDefinition kDef_MEAZON_DINRAIL;

const PreparedDefinition* const kMeazonRegistry[] = {
    &kDef_MEAZON_BIZY_PLUG,
    &kDef_MEAZON_DINRAIL,
};
const std::size_t kMeazonRegistryCount = sizeof(kMeazonRegistry) / sizeof(kMeazonRegistry[0]);

}  // namespace zhc::devices::meazon
