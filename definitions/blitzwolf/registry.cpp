// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated blitzwolf registry.
#include "zhc/devices/blitzwolf_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::blitzwolf {

extern const PreparedDefinition kDef_BW_SS7_1gang;
extern const PreparedDefinition kDef_BW_SS7_2gang;
extern const PreparedDefinition kDef_BW_IS3;
extern const PreparedDefinition kDef_BW_IS9;

const PreparedDefinition* const kBlitzwolfRegistry[] = {
    &kDef_BW_SS7_1gang,
    &kDef_BW_SS7_2gang,
    &kDef_BW_IS3,
    &kDef_BW_IS9,
};
const std::size_t kBlitzwolfRegistryCount = sizeof(kBlitzwolfRegistry) / sizeof(kBlitzwolfRegistry[0]);

}  // namespace zhc::devices::blitzwolf
