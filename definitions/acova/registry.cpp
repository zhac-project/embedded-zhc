// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated acova registry.
#include "zhc/devices/acova_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::acova {

extern const PreparedDefinition kDef_ALCANTARA2;
extern const PreparedDefinition kDef_TAFFETAS2_PERCALE2;
extern const PreparedDefinition kDef_IHC_Enki;

const PreparedDefinition* const kAcovaRegistry[] = {
    &kDef_ALCANTARA2,
    &kDef_TAFFETAS2_PERCALE2,
    &kDef_IHC_Enki,
};
const std::size_t kAcovaRegistryCount = sizeof(kAcovaRegistry) / sizeof(kAcovaRegistry[0]);

}  // namespace zhc::devices::acova
