// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated xal registry.
#include "zhc/devices/xal_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::xal {

extern const PreparedDefinition kDef_D050_0131558M;
extern const PreparedDefinition kDef_D050_0511558F;
extern const PreparedDefinition kDef_D050_1212558H;

const PreparedDefinition* const kXalRegistry[] = {
    &kDef_D050_0131558M,
    &kDef_D050_0511558F,
    &kDef_D050_1212558H,
};
const std::size_t kXalRegistryCount = sizeof(kXalRegistry) / sizeof(kXalRegistry[0]);

}  // namespace zhc::devices::xal
