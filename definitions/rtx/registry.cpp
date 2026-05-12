// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated rtx registry.
#include "zhc/devices/rtx_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::rtx {

extern const PreparedDefinition kDef_ZVG1;
extern const PreparedDefinition kDef_ZMS4;

const PreparedDefinition* const kRtxRegistry[] = {
    &kDef_ZVG1,
    &kDef_ZMS4,
};
const std::size_t kRtxRegistryCount = sizeof(kRtxRegistry) / sizeof(kRtxRegistry[0]);

}  // namespace zhc::devices::rtx
