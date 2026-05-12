// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated tis_control registry.
#include "zhc/devices/tis_control_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::tis_control {

extern const PreparedDefinition kDef_BEE_PIR_1;

const PreparedDefinition* const kTisControlRegistry[] = {
    &kDef_BEE_PIR_1,
};
const std::size_t kTisControlRegistryCount = sizeof(kTisControlRegistry) / sizeof(kTisControlRegistry[0]);

}  // namespace zhc::devices::tis_control
