// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated perenio registry.
#include "zhc/devices/perenio_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::perenio {

extern const PreparedDefinition kDef_PEHPL0X;
extern const PreparedDefinition kDef_PEHWE20;
extern const PreparedDefinition kDef_PECLS01;
extern const PreparedDefinition kDef_PECMS01;
extern const PreparedDefinition kDef_PECWS01;

const PreparedDefinition* const kPerenioRegistry[] = {
    &kDef_PEHPL0X,
    &kDef_PEHWE20,
    &kDef_PECLS01,
    &kDef_PECMS01,
    &kDef_PECWS01,
};
const std::size_t kPerenioRegistryCount = sizeof(kPerenioRegistry) / sizeof(kPerenioRegistry[0]);

}  // namespace zhc::devices::perenio
