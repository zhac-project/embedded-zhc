// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated mercator registry.
#include "zhc/devices/mercator_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::mercator {

extern const PreparedDefinition kDef_SPP02G;
extern const PreparedDefinition kDef_SPP02GIP;
extern const PreparedDefinition kDef_SPP04G;
extern const PreparedDefinition kDef_SPPUSB02;
extern const PreparedDefinition kDef_SSW03G;
extern const PreparedDefinition kDef_SSWF01G;
extern const PreparedDefinition kDef_SMA02P;
extern const PreparedDefinition kDef_SMA03P;
extern const PreparedDefinition kDef_SMA04P;
extern const PreparedDefinition kDef_SMD4109W_RGB_ZB;

const PreparedDefinition* const kMercatorRegistry[] = {
    &kDef_SPP02G,
    &kDef_SPP02GIP,
    &kDef_SPP04G,
    &kDef_SPPUSB02,
    &kDef_SSW03G,
    &kDef_SSWF01G,
    &kDef_SMA02P,
    &kDef_SMA03P,
    &kDef_SMA04P,
    &kDef_SMD4109W_RGB_ZB,
};
const std::size_t kMercatorRegistryCount = sizeof(kMercatorRegistry) / sizeof(kMercatorRegistry[0]);

}  // namespace zhc::devices::mercator
