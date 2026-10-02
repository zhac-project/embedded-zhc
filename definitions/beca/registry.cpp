// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// beca registry.
#include "zhc/devices/beca_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::beca {

extern const PreparedDefinition kDef_BVRF_L001;

const PreparedDefinition* const kBecaRegistry[] = {
    &kDef_BVRF_L001,
};
const std::size_t kBecaRegistryCount = sizeof(kBecaRegistry) / sizeof(kBecaRegistry[0]);

}  // namespace zhc::devices::beca
