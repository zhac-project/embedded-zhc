// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated plaid registry.
#include "zhc/devices/plaid_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::plaid {

extern const PreparedDefinition kDef_PS_SPRZMS_SLP3;

const PreparedDefinition* const kPlaidRegistry[] = {
    &kDef_PS_SPRZMS_SLP3,
};
const std::size_t kPlaidRegistryCount = sizeof(kPlaidRegistry) / sizeof(kPlaidRegistry[0]);

}  // namespace zhc::devices::plaid
