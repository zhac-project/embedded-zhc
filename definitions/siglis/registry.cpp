// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated siglis registry.
#include "zhc/devices/siglis_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::siglis {

extern const PreparedDefinition kDef_ZFP_1A_CH;
extern const PreparedDefinition kDef_ZFU_1D_CH;

const PreparedDefinition* const kSiglisRegistry[] = {
    &kDef_ZFP_1A_CH,
    &kDef_ZFU_1D_CH,
};
const std::size_t kSiglisRegistryCount = sizeof(kSiglisRegistry) / sizeof(kSiglisRegistry[0]);

}  // namespace zhc::devices::siglis
