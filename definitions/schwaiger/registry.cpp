// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated schwaiger registry.
#include "zhc/devices/schwaiger_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::schwaiger {

extern const PreparedDefinition kDef_HAL300;
extern const PreparedDefinition kDef_HAL400;
extern const PreparedDefinition kDef_HAL500;
extern const PreparedDefinition kDef_HAL550;
extern const PreparedDefinition kDef_HAL600;
extern const PreparedDefinition kDef_HAL800;
extern const PreparedDefinition kDef_ZHS_15;

const PreparedDefinition* const kSchwaigerRegistry[] = {
    &kDef_HAL300,
    &kDef_HAL400,
    &kDef_HAL500,
    &kDef_HAL550,
    &kDef_HAL600,
    &kDef_HAL800,
    &kDef_ZHS_15,
};
const std::size_t kSchwaigerRegistryCount = sizeof(kSchwaigerRegistry) / sizeof(kSchwaigerRegistry[0]);

}  // namespace zhc::devices::schwaiger
