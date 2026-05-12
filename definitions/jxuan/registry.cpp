// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated jxuan registry.
#include "zhc/devices/jxuan_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::jxuan {

extern const PreparedDefinition kDef_SPZ01;
extern const PreparedDefinition kDef_WSZ01;
extern const PreparedDefinition kDef_DSZ01;
extern const PreparedDefinition kDef_PRZ01;

const PreparedDefinition* const kJxuanRegistry[] = {
    &kDef_SPZ01,
    &kDef_WSZ01,
    &kDef_DSZ01,
    &kDef_PRZ01,
};
const std::size_t kJxuanRegistryCount = sizeof(kJxuanRegistry) / sizeof(kJxuanRegistry[0]);

}  // namespace zhc::devices::jxuan
