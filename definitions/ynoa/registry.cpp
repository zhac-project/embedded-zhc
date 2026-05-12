// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated ynoa registry.
#include "zhc/devices/ynoa_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::ynoa {

extern const PreparedDefinition kDef_D8718801528204;
extern const PreparedDefinition kDef_D8718801528273;
extern const PreparedDefinition kDef_D8718801528334;
extern const PreparedDefinition kDef_LA_5KEY_RGBW;
extern const PreparedDefinition kDef_LA_A60_CCT;
extern const PreparedDefinition kDef_LA_GU10_RGBW;
extern const PreparedDefinition kDef_LA_PLUG_10Amp;

const PreparedDefinition* const kYnoaRegistry[] = {
    &kDef_D8718801528204,
    &kDef_D8718801528273,
    &kDef_D8718801528334,
    &kDef_LA_5KEY_RGBW,
    &kDef_LA_A60_CCT,
    &kDef_LA_GU10_RGBW,
    &kDef_LA_PLUG_10Amp,
};
const std::size_t kYnoaRegistryCount = sizeof(kYnoaRegistry) / sizeof(kYnoaRegistry[0]);

}  // namespace zhc::devices::ynoa
