// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated multir registry.
#include "zhc/devices/multir_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::multir {

extern const PreparedDefinition kDef_MIR_SO100;
extern const PreparedDefinition kDef_MIR_IR100;
extern const PreparedDefinition kDef_MIR_MC100;
extern const PreparedDefinition kDef_MIR_SM100_E;
extern const PreparedDefinition kDef_MIR_SM200;
extern const PreparedDefinition kDef_MIR_TE600;
extern const PreparedDefinition kDef_MIR_WA100;

const PreparedDefinition* const kMultirRegistry[] = {
    &kDef_MIR_SO100,
    &kDef_MIR_IR100,
    &kDef_MIR_MC100,
    &kDef_MIR_SM100_E,
    &kDef_MIR_SM200,
    &kDef_MIR_TE600,
    &kDef_MIR_WA100,
};
const std::size_t kMultirRegistryCount = sizeof(kMultirRegistry) / sizeof(kMultirRegistry[0]);

}  // namespace zhc::devices::multir
