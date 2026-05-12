// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated javis registry.
#include "zhc/devices/javis_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::javis {

extern const PreparedDefinition kDef_JS_SLK2_ZB;
extern const PreparedDefinition kDef_JS_MC_SENSOR_ZB;

const PreparedDefinition* const kJavisRegistry[] = {
    &kDef_JS_SLK2_ZB,
    &kDef_JS_MC_SENSOR_ZB,
};
const std::size_t kJavisRegistryCount = sizeof(kJavisRegistry) / sizeof(kJavisRegistry[0]);

}  // namespace zhc::devices::javis
