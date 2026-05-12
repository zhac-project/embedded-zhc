// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated visonic registry.
#include "zhc/devices/visonic_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::visonic {

extern const PreparedDefinition kDef_GB_540;
extern const PreparedDefinition kDef_MCT_302_SMA;
extern const PreparedDefinition kDef_MCT_340_E;
extern const PreparedDefinition kDef_MCT_340_SMA;
extern const PreparedDefinition kDef_MCT_350_SMA;
extern const PreparedDefinition kDef_MCT_370_SMA;
extern const PreparedDefinition kDef_MP_840;
extern const PreparedDefinition kDef_MP_841;

const PreparedDefinition* const kVisonicRegistry[] = {
    &kDef_GB_540,
    &kDef_MCT_302_SMA,
    &kDef_MCT_340_E,
    &kDef_MCT_340_SMA,
    &kDef_MCT_350_SMA,
    &kDef_MCT_370_SMA,
    &kDef_MP_840,
    &kDef_MP_841,
};
const std::size_t kVisonicRegistryCount = sizeof(kVisonicRegistry) / sizeof(kVisonicRegistry[0]);

}  // namespace zhc::devices::visonic
