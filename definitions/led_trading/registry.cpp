// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated led_trading registry.
#include "zhc/devices/led_trading_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::led_trading {

extern const PreparedDefinition kDef_D9133;
extern const PreparedDefinition kDef_D9134;
extern const PreparedDefinition kDef_D9135;
extern const PreparedDefinition kDef_HK_LN_DIM_A;

const PreparedDefinition* const kLedTradingRegistry[] = {
    &kDef_D9133,
    &kDef_D9134,
    &kDef_D9135,
    &kDef_HK_LN_DIM_A,
};
const std::size_t kLedTradingRegistryCount = sizeof(kLedTradingRegistry) / sizeof(kLedTradingRegistry[0]);

}  // namespace zhc::devices::led_trading
