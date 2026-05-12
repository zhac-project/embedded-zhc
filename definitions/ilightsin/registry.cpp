// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated ilightsin registry.
#include "zhc/devices/ilightsin_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::ilightsin {

extern const PreparedDefinition kDef_HLC610;
extern const PreparedDefinition kDef_HSSA18_Z_MID;

const PreparedDefinition* const kIlightsinRegistry[] = {
    &kDef_HLC610,
    &kDef_HSSA18_Z_MID,
};
const std::size_t kIlightsinRegistryCount = sizeof(kIlightsinRegistry) / sizeof(kIlightsinRegistry[0]);

}  // namespace zhc::devices::ilightsin
