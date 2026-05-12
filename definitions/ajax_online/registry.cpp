// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated ajax_online registry.
#include "zhc/devices/ajax_online_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::ajax_online {

extern const PreparedDefinition kDef_AJ_ZB_GU10;
extern const PreparedDefinition kDef_AJ_ZIGPROA60;
extern const PreparedDefinition kDef_Aj_Zigbee_Led_Strip;
extern const PreparedDefinition kDef_ZB_A60_RGBCW;
extern const PreparedDefinition kDef_ZB_CCT_Filament;
extern const PreparedDefinition kDef_AJ_RGBCCT_CTRL;

const PreparedDefinition* const kAjaxOnlineRegistry[] = {
    &kDef_AJ_ZB_GU10,
    &kDef_AJ_ZIGPROA60,
    &kDef_Aj_Zigbee_Led_Strip,
    &kDef_ZB_A60_RGBCW,
    &kDef_ZB_CCT_Filament,
    &kDef_AJ_RGBCCT_CTRL,
};
const std::size_t kAjaxOnlineRegistryCount = sizeof(kAjaxOnlineRegistry) / sizeof(kAjaxOnlineRegistry[0]);

}  // namespace zhc::devices::ajax_online
