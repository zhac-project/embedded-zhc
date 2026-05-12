// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated swann registry.
#include "zhc/devices/swann_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::swann {

extern const PreparedDefinition kDef_SWO_KEF1PA;
extern const PreparedDefinition kDef_SWO_MOS1PA;
extern const PreparedDefinition kDef_SWO_WDS1PA;

const PreparedDefinition* const kSwannRegistry[] = {
    &kDef_SWO_KEF1PA,
    &kDef_SWO_MOS1PA,
    &kDef_SWO_WDS1PA,
};
const std::size_t kSwannRegistryCount = sizeof(kSwannRegistry) / sizeof(kSwannRegistry[0]);

}  // namespace zhc::devices::swann
