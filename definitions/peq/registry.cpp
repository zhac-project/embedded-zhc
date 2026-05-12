// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated peq registry.
#include "zhc/devices/peq_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::peq {

extern const PreparedDefinition kDef_D3300_P;

const PreparedDefinition* const kPeqRegistry[] = {
    &kDef_D3300_P,
};
const std::size_t kPeqRegistryCount = sizeof(kPeqRegistry) / sizeof(kPeqRegistry[0]);

}  // namespace zhc::devices::peq
