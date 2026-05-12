// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated edp registry.
#include "zhc/devices/edp_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::edp {

extern const PreparedDefinition kDef_PLUG_EDP_RE_DY;
extern const PreparedDefinition kDef_SWITCH_EDP_RE_DY;

const PreparedDefinition* const kEdpRegistry[] = {
    &kDef_PLUG_EDP_RE_DY,
    &kDef_SWITCH_EDP_RE_DY,
};
const std::size_t kEdpRegistryCount = sizeof(kEdpRegistry) / sizeof(kEdpRegistry[0]);

}  // namespace zhc::devices::edp
