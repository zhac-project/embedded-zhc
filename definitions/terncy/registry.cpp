// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated terncy registry.
#include "zhc/devices/terncy_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::terncy {

extern const PreparedDefinition kDef_CL001;
extern const PreparedDefinition kDef_DL001;
extern const PreparedDefinition kDef_TERNCY_LS01;
extern const PreparedDefinition kDef_TERNCY_PP01;
extern const PreparedDefinition kDef_TERNCY_SD01;
extern const PreparedDefinition kDef_TERNCY_WS01;
extern const PreparedDefinition kDef_TERNCY_DC01;

const PreparedDefinition* const kTerncyRegistry[] = {
    &kDef_CL001,
    &kDef_DL001,
    &kDef_TERNCY_LS01,
    &kDef_TERNCY_PP01,
    &kDef_TERNCY_SD01,
    &kDef_TERNCY_WS01,
    &kDef_TERNCY_DC01,
};
const std::size_t kTerncyRegistryCount = sizeof(kTerncyRegistry) / sizeof(kTerncyRegistry[0]);

}  // namespace zhc::devices::terncy
