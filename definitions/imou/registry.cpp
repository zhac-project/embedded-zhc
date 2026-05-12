// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated imou registry.
#include "zhc/devices/imou_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::imou {

extern const PreparedDefinition kDef_ZD1_EN;
extern const PreparedDefinition kDef_ZE1_EN;
extern const PreparedDefinition kDef_ZGA1_EN;
extern const PreparedDefinition kDef_ZP1_EN;
extern const PreparedDefinition kDef_ZR1_EN;
extern const PreparedDefinition kDef_ZTM1_EN;

const PreparedDefinition* const kImouRegistry[] = {
    &kDef_ZD1_EN,
    &kDef_ZE1_EN,
    &kDef_ZGA1_EN,
    &kDef_ZP1_EN,
    &kDef_ZR1_EN,
    &kDef_ZTM1_EN,
};
const std::size_t kImouRegistryCount = sizeof(kImouRegistry) / sizeof(kImouRegistry[0]);

}  // namespace zhc::devices::imou
