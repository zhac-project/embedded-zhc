// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated repenic_ltd registry.
#include "zhc/devices/repenic_ltd_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::repenic_ltd {

extern const PreparedDefinition kDef_RD_250ZG;
extern const PreparedDefinition kDef_RS_T03AZG;

const PreparedDefinition* const kRepenicLtdRegistry[] = {
    &kDef_RD_250ZG,
    &kDef_RS_T03AZG,
};
const std::size_t kRepenicLtdRegistryCount = sizeof(kRepenicLtdRegistry) / sizeof(kRepenicLtdRegistry[0]);

}  // namespace zhc::devices::repenic_ltd
