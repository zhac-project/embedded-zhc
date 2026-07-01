// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated wirenboard registry.
#include "zhc/devices/wirenboard_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::wirenboard {

extern const PreparedDefinition kDef_WB_MSW_ZIGBEE_v_3;
extern const PreparedDefinition kDef_WB_MSW_ZIGBEE_v_4;
extern const PreparedDefinition kDef_WB_MSW_ZIGBEE_v_4_official;

const PreparedDefinition* const kWirenboardRegistry[] = {
    &kDef_WB_MSW_ZIGBEE_v_3,
    &kDef_WB_MSW_ZIGBEE_v_4,
    &kDef_WB_MSW_ZIGBEE_v_4_official,
};
const std::size_t kWirenboardRegistryCount = sizeof(kWirenboardRegistry) / sizeof(kWirenboardRegistry[0]);

}  // namespace zhc::devices::wirenboard
