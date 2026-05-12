// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated lifecontrol registry.
#include "zhc/devices/lifecontrol_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::lifecontrol {

extern const PreparedDefinition kDef_MCLH_02;
extern const PreparedDefinition kDef_MCLH_03;
extern const PreparedDefinition kDef_MCLH_04;
extern const PreparedDefinition kDef_MCLH_05;
extern const PreparedDefinition kDef_MCLH_07;
extern const PreparedDefinition kDef_MCLH_08;

const PreparedDefinition* const kLifecontrolRegistry[] = {
    &kDef_MCLH_02,
    &kDef_MCLH_03,
    &kDef_MCLH_04,
    &kDef_MCLH_05,
    &kDef_MCLH_07,
    &kDef_MCLH_08,
};
const std::size_t kLifecontrolRegistryCount = sizeof(kLifecontrolRegistry) / sizeof(kLifecontrolRegistry[0]);

}  // namespace zhc::devices::lifecontrol
