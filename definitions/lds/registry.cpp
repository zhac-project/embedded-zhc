// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated lds registry.
#include "zhc/devices/lds_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::lds {

extern const PreparedDefinition kDef_FWBulb03UK;
extern const PreparedDefinition kDef_ZBT_RGBWLight_A0000;
extern const PreparedDefinition kDef_PFMOT001;

const PreparedDefinition* const kLdsRegistry[] = {
    &kDef_FWBulb03UK,
    &kDef_ZBT_RGBWLight_A0000,
    &kDef_PFMOT001,
};
const std::size_t kLdsRegistryCount = sizeof(kLdsRegistry) / sizeof(kLdsRegistry[0]);

}  // namespace zhc::devices::lds
