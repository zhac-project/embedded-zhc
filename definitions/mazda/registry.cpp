// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated mazda registry.
#include "zhc/devices/mazda_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::mazda {

extern const PreparedDefinition kDefMaz__TZE204_k6rdmisz;
extern const PreparedDefinition kDefMaz__TZE284_k6rdmisz;
extern const PreparedDefinition kDef_TR_M2Z;

const PreparedDefinition* const kMazdaRegistry[] = {
    &kDefMaz__TZE204_k6rdmisz,
    &kDefMaz__TZE284_k6rdmisz,
    &kDef_TR_M2Z,
};
const std::size_t kMazdaRegistryCount = sizeof(kMazdaRegistry) / sizeof(kMazdaRegistry[0]);

}  // namespace zhc::devices::mazda
