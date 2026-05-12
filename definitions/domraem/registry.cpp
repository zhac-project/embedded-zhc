// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated domraem registry.
#include "zhc/devices/domraem_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::domraem {

extern const PreparedDefinition kDef_DOM_Z_105P_CCT;
extern const PreparedDefinition kDef_DOM_Z_105P_DIMMER;
extern const PreparedDefinition kDef_DOM_Z_105P_RGB;
extern const PreparedDefinition kDef_DOM_Z_105P_RGBCCT;
extern const PreparedDefinition kDef_DOM_Z_105P_RGBW;
extern const PreparedDefinition kDef_DOM_Z_105P_WW_CW;

const PreparedDefinition* const kDomraemRegistry[] = {
    &kDef_DOM_Z_105P_CCT,
    &kDef_DOM_Z_105P_DIMMER,
    &kDef_DOM_Z_105P_RGB,
    &kDef_DOM_Z_105P_RGBCCT,
    &kDef_DOM_Z_105P_RGBW,
    &kDef_DOM_Z_105P_WW_CW,
};
const std::size_t kDomraemRegistryCount = sizeof(kDomraemRegistry) / sizeof(kDomraemRegistry[0]);

}  // namespace zhc::devices::domraem
